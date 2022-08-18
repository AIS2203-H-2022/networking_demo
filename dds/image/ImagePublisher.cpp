
#include "ImagePubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace eprosima::fastdds::dds;

class ImagePublisher {

private:
    DomainParticipant *participant_;
    Publisher *publisher_;
    Topic *topic_;
    DataWriter *writer_;
    TypeSupport type_;
    VideoCapture capture_;

    class PubListener : public DataWriterListener {
    public:
        PubListener()
            : matched_(0) {}

        void on_publication_matched(
                DataWriter *,
                const PublicationMatchedStatus &info) override {
            if (info.current_count_change == 1) {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            } else if (info.current_count_change == -1) {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change
                          << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
            }
        }

        std::atomic_int matched_;

    } listener_;

public:
    ImagePublisher()
        : participant_(nullptr),
          publisher_(nullptr),
          topic_(nullptr),
          writer_(nullptr),
          type_(new ImageStructPubSubType()),
          capture_(0) {}

    //!Initialize the publisher
    bool init() {

        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (!participant_) {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the publications Topic
        TopicQos topicQos;
        topicQos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
        topic_ = participant_->create_topic("ImageTopic", "ImageStruct", topicQos);

        if (!topic_) {
            return false;
        }

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

        if (!publisher_) {
            return false;
        }

        // Create the DataWriter
        DataWriterQos writerQos;
        writerQos.endpoint().history_memory_policy = eprosima::fastrtps::rtps::DYNAMIC_REUSABLE_MEMORY_MODE;
        writerQos.reliability().kind = ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS;
        writer_ = publisher_->create_datawriter(topic_, writerQos, &listener_);

        if (!writer_) {
            return false;
        }
        return true;
    }

    //!Run the Publisher
    void run() {

        bool stop = false;
        std::thread t([&stop] {
            std::cout << "Press a key to continue..." << std::endl;
            while (std::cin.get() != '\n') {}
            stop = true;
        });

        while (!stop) {
            Mat img;
            ImageStruct is;

            if (listener_.matched_ > 0) {

                capture_.read(img);

                std::vector<uint8_t> buf;
                imencode(".jpg", img, buf);

                is.data(std::move(buf));
                writer_->write(&is);

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        t.join();
    }

    ~ImagePublisher() {
        if (writer_) {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_) {
            participant_->delete_publisher(publisher_);
        }
        if (topic_) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
};

int main() {
    std::cout << "Starting image publisher." << std::endl;

    ImagePublisher pub;
    if (pub.init()) {
        pub.run();
    }

    return 0;
}
