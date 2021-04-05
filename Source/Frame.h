#ifndef _H_FRAME_
#define _H_FRAME_

template<unsigned int N>
class Frame {
public:
	float m_value[N];
	float m_in[N];
	float m_out[N];
	float m_time;
};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;

#endif