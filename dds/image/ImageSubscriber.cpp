
#include "ImagePubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace eprosima::fastdds::dds;

class ImageSubscriber {

private:
    DomainParticipant *participant_;
    Subscriber *subscriber_;
    DataReader *reader_;
    Topic *topic_;
    TypeSupport type_;

    class SubListener : public DataReaderListener {
    public:

        void on_subscription_matched(
                DataReader *,
                const SubscriptionMatchedStatus &info) override {
            if (info.current_count_change == 1) {
                std::cout << "Subscriber matched." << std::endl;
            } else if (info.current_count_change == -1) {
                std::cout << "Subscriber unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change
                          << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        void on_sample_lost(DataReader *reader, const SampleLostStatus &status) override {
            DataReaderListener::on_sample_lost(reader, status);
        }

        void on_requested_deadline_missed(DataReader *reader, const RequestedDeadlineMissedStatus &status) override {
            DataReaderListener::on_requested_deadline_missed(reader, status);
        }
        void on_liveliness_changed(DataReader *reader, const LivelinessChangedStatus &status) override {
            DataReaderListener::on_liveliness_changed(reader, status);
        }
        void on_sample_rejected(DataReader *reader, const SampleRejectedStatus &status) override {
            DataReaderListener::on_sample_rejected(reader, status);
        }
        void on_requested_incompatible_qos(DataReader *reader, const RequestedIncompatibleQosStatus &status) override {
            DataReaderListener::on_requested_incompatible_qos(reader, status);
        }



        void on_data_available(DataReader *reader) override {
            SampleInfo info;
            if (reader->take_next_sample(&image_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
                    auto& data = image_.data();
                    Mat image = imdecode(data, IMREAD_COLOR);
//                    imshow("img", image);
//                    waitKey(1);
//                    samples_++;
                    std::cout << "Message with size: " << image_.data().size()
                              << " RECEIVED." << std::endl;
                }
            } else {
                std::cout << "ok.." << std::endl;
            }
        }

        ImageStruct image_;

    } listener_;

public:
    ImageSubscriber()
        : participant_(nullptr),
          subscriber_(nullptr),
          topic_(nullptr),
          reader_(nullptr),
          type_(new ImageStructPubSubType()) {}

    //!Initialize the subscriber
    bool init() {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (!participant_) {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("ImageTopic", "ImageStruct", TOPIC_QOS_DEFAULT);

        if (!topic_) {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (!subscriber_) {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        if (reader_ == nullptr) {
            return false;
        }

        return true;
    }

    //!Run the Subscriber
    void run() const {

        bool stop = false;
        std::thread t([&stop]{
            std::cout << "Press a key to continue..." << std::endl;
            while (std::cin.get() != '\n') {}
            stop = true;
        });

        while (!stop) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        t.join();
    }

    ~ImageSubscriber() {
        if (reader_) {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_) {
            participant_->delete_topic(topic_);
        }
        if (subscriber_) {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
};

int main() {
    std::cout << "Starting image subscriber." << std::endl;

    ImageSubscriber sub;
    if (sub.init()) {
        sub.run();
    }

    return 0;
}
