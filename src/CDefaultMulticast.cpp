#include<marlin/multicast/DefaultMulticastClient.hpp>
#include<marlin/multicast/MulticastClientWrapper.h>

using namespace marlin::multicast;
using namespace marlin::net;


class MulticastDelegateWrapper {
private:
	// MulticastClientDelegate_t* mc_del;

public:
	MulticastClientDelegate_t* mc_del;

	void did_recv_message(
		DefaultMulticastClient<MulticastDelegateWrapper> &,
		Buffer &&message,
		std::string &channel,
		uint64_t message_id
	) {
		SPDLOG_INFO(
			"Did recv message: {}",
			std::string(message.data(), message.data() + message.size())
		);

		SPDLOG_INFO(
			"channel: {}, message_id: {}",
			channel,
			message_id
		);

		mc_del->did_recv_message();
	}

	void did_subscribe(
		DefaultMulticastClient<MulticastDelegateWrapper> &client,
		std::string &channel
	) {
		client.ps.send_message_on_channel(channel, "Hello!", 6);
	}

	void did_unsubscribe(
		DefaultMulticastClient<MulticastDelegateWrapper> &,
		std::string &
	) {}
};


struct MulticastClientWrapper {
	void *obj;
};

MulticastClientWrapper_t* create_multicastclientwrapper() {
	DefaultMulticastClientOptions clop1 {
		{"goldfish"},
		"127.0.0.1:9002"
	};

	MulticastClientWrapper_t *mc_w;
	mc_w = (__typeof__(mc_w))malloc(sizeof(*mc_w));

	mc_w->obj = new DefaultMulticastClient<MulticastDelegateWrapper> (clop1);

	return mc_w;
}

void setDelegate(MulticastClientWrapper_t* mc_w, MulticastClientDelegate_t* mc_d) {
	MulticastDelegateWrapper* mc_d_w = new MulticastDelegateWrapper();
	mc_d_w->mc_del = mc_d;


	DefaultMulticastClient<MulticastDelegateWrapper> *obj;
	obj = reinterpret_cast< DefaultMulticastClient<MulticastDelegateWrapper> *>(mc_w->obj);

	obj->delegate = mc_d_w;

}


