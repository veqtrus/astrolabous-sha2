#ifndef ASTROLABOUS_SHA2_H
#define ASTROLABOUS_SHA2_H

#ifdef __OPENCL_VERSION__

#define static
#define uint32_t unsigned int

#define RR(x, n) rotate((uint32_t) x, (uint32_t) (32 - n))
#define CH(e, f, g) bitselect(g, f, e)

#else

#include <stdint.h>

#define RR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(e, f, g) (((g ^ f) & e) ^ g)

#endif

#define MAJ(a, b, c) (((a | b) & c) | (a & b))

#define SCHED(i) w[i & 15] += w[(i + 9) & 15] \
	+ (RR(w[(i + 1) & 15], 7) ^ RR(w[(i + 1) & 15], 18) ^ (w[(i + 1) & 15] >> 3)) \
	+ (RR(w[(i + 14) & 15], 17) ^ RR(w[(i + 14) & 15], 19) ^ (w[(i + 14) & 15] >> 10))

#define ROUND(a, b, c, d, e, f, g, h, i, k) \
	h += CH(e, f, g) + k + w[i & 15] \
		+ (RR(e, 6) ^ RR(e, 11) ^ RR(e, 25)); \
	d += h; \
	h += MAJ(a, b, c) + (RR(a, 2) ^ RR(a, 13) ^ RR(a, 22))

static void sha2(uint32_t *buf)
{
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t w[16];
	
	w[0] = buf[0];
	w[1] = buf[1];
	w[2] = buf[2];
	w[3] = buf[3];
	w[4] = buf[4];
	w[5] = buf[5];
	w[6] = buf[6];
	w[7] = buf[7];
	w[8] = 0x80000000U;
	w[9] = 0;
	w[10] = 0;
	w[11] = 0;
	w[12] = 0;
	w[13] = 0;
	w[14] = 0;
	w[15] = 256;
	
	a = 0x6A09E667U;
	b = 0xBB67AE85U;
	c = 0x3C6EF372U;
	d = 0xA54FF53AU;
	e = 0x510E527FU;
	f = 0x9B05688CU;
	g = 0x1F83D9ABU;
	h = 0x5BE0CD19U;
	
	ROUND(a, b, c, d, e, f, g, h, 0, 0x428A2F98U);
	ROUND(h, a, b, c, d, e, f, g, 1, 0x71374491U);
	ROUND(g, h, a, b, c, d, e, f, 2, 0xB5C0FBCFU);
	ROUND(f, g, h, a, b, c, d, e, 3, 0xE9B5DBA5U);
	ROUND(e, f, g, h, a, b, c, d, 4, 0x3956C25BU);
	ROUND(d, e, f, g, h, a, b, c, 5, 0x59F111F1U);
	ROUND(c, d, e, f, g, h, a, b, 6, 0x923F82A4U);
	ROUND(b, c, d, e, f, g, h, a, 7, 0xAB1C5ED5U);
	ROUND(a, b, c, d, e, f, g, h, 8, 0xD807AA98U);
	ROUND(h, a, b, c, d, e, f, g, 9, 0x12835B01U);
	ROUND(g, h, a, b, c, d, e, f, 10, 0x243185BEU);
	ROUND(f, g, h, a, b, c, d, e, 11, 0x550C7DC3U);
	ROUND(e, f, g, h, a, b, c, d, 12, 0x72BE5D74U);
	ROUND(d, e, f, g, h, a, b, c, 13, 0x80DEB1FEU);
	ROUND(c, d, e, f, g, h, a, b, 14, 0x9BDC06A7U);
	ROUND(b, c, d, e, f, g, h, a, 15, 0xC19BF174U);
	SCHED(16);
	ROUND(a, b, c, d, e, f, g, h, 16, 0xE49B69C1U);
	SCHED(17);
	ROUND(h, a, b, c, d, e, f, g, 17, 0xEFBE4786U);
	SCHED(18);
	ROUND(g, h, a, b, c, d, e, f, 18, 0x0FC19DC6U);
	SCHED(19);
	ROUND(f, g, h, a, b, c, d, e, 19, 0x240CA1CCU);
	SCHED(20);
	ROUND(e, f, g, h, a, b, c, d, 20, 0x2DE92C6FU);
	SCHED(21);
	ROUND(d, e, f, g, h, a, b, c, 21, 0x4A7484AAU);
	SCHED(22);
	ROUND(c, d, e, f, g, h, a, b, 22, 0x5CB0A9DCU);
	SCHED(23);
	ROUND(b, c, d, e, f, g, h, a, 23, 0x76F988DAU);
	SCHED(24);
	ROUND(a, b, c, d, e, f, g, h, 24, 0x983E5152U);
	SCHED(25);
	ROUND(h, a, b, c, d, e, f, g, 25, 0xA831C66DU);
	SCHED(26);
	ROUND(g, h, a, b, c, d, e, f, 26, 0xB00327C8U);
	SCHED(27);
	ROUND(f, g, h, a, b, c, d, e, 27, 0xBF597FC7U);
	SCHED(28);
	ROUND(e, f, g, h, a, b, c, d, 28, 0xC6E00BF3U);
	SCHED(29);
	ROUND(d, e, f, g, h, a, b, c, 29, 0xD5A79147U);
	SCHED(30);
	ROUND(c, d, e, f, g, h, a, b, 30, 0x06CA6351U);
	SCHED(31);
	ROUND(b, c, d, e, f, g, h, a, 31, 0x14292967U);
	SCHED(32);
	ROUND(a, b, c, d, e, f, g, h, 32, 0x27B70A85U);
	SCHED(33);
	ROUND(h, a, b, c, d, e, f, g, 33, 0x2E1B2138U);
	SCHED(34);
	ROUND(g, h, a, b, c, d, e, f, 34, 0x4D2C6DFCU);
	SCHED(35);
	ROUND(f, g, h, a, b, c, d, e, 35, 0x53380D13U);
	SCHED(36);
	ROUND(e, f, g, h, a, b, c, d, 36, 0x650A7354U);
	SCHED(37);
	ROUND(d, e, f, g, h, a, b, c, 37, 0x766A0ABBU);
	SCHED(38);
	ROUND(c, d, e, f, g, h, a, b, 38, 0x81C2C92EU);
	SCHED(39);
	ROUND(b, c, d, e, f, g, h, a, 39, 0x92722C85U);
	SCHED(40);
	ROUND(a, b, c, d, e, f, g, h, 40, 0xA2BFE8A1U);
	SCHED(41);
	ROUND(h, a, b, c, d, e, f, g, 41, 0xA81A664BU);
	SCHED(42);
	ROUND(g, h, a, b, c, d, e, f, 42, 0xC24B8B70U);
	SCHED(43);
	ROUND(f, g, h, a, b, c, d, e, 43, 0xC76C51A3U);
	SCHED(44);
	ROUND(e, f, g, h, a, b, c, d, 44, 0xD192E819U);
	SCHED(45);
	ROUND(d, e, f, g, h, a, b, c, 45, 0xD6990624U);
	SCHED(46);
	ROUND(c, d, e, f, g, h, a, b, 46, 0xF40E3585U);
	SCHED(47);
	ROUND(b, c, d, e, f, g, h, a, 47, 0x106AA070U);
	SCHED(48);
	ROUND(a, b, c, d, e, f, g, h, 48, 0x19A4C116U);
	SCHED(49);
	ROUND(h, a, b, c, d, e, f, g, 49, 0x1E376C08U);
	SCHED(50);
	ROUND(g, h, a, b, c, d, e, f, 50, 0x2748774CU);
	SCHED(51);
	ROUND(f, g, h, a, b, c, d, e, 51, 0x34B0BCB5U);
	SCHED(52);
	ROUND(e, f, g, h, a, b, c, d, 52, 0x391C0CB3U);
	SCHED(53);
	ROUND(d, e, f, g, h, a, b, c, 53, 0x4ED8AA4AU);
	SCHED(54);
	ROUND(c, d, e, f, g, h, a, b, 54, 0x5B9CCA4FU);
	SCHED(55);
	ROUND(b, c, d, e, f, g, h, a, 55, 0x682E6FF3U);
	SCHED(56);
	ROUND(a, b, c, d, e, f, g, h, 56, 0x748F82EEU);
	SCHED(57);
	ROUND(h, a, b, c, d, e, f, g, 57, 0x78A5636FU);
	SCHED(58);
	ROUND(g, h, a, b, c, d, e, f, 58, 0x84C87814U);
	SCHED(59);
	ROUND(f, g, h, a, b, c, d, e, 59, 0x8CC70208U);
	SCHED(60);
	ROUND(e, f, g, h, a, b, c, d, 60, 0x90BEFFFAU);
	SCHED(61);
	ROUND(d, e, f, g, h, a, b, c, 61, 0xA4506CEBU);
	SCHED(62);
	ROUND(c, d, e, f, g, h, a, b, 62, 0xBEF9A3F7U);
	SCHED(63);
	ROUND(b, c, d, e, f, g, h, a, 63, 0xC67178F2U);
	
	buf[0] = a + 0x6A09E667U;
	buf[1] = b + 0xBB67AE85U;
	buf[2] = c + 0x3C6EF372U;
	buf[3] = d + 0xA54FF53AU;
	buf[4] = e + 0x510E527FU;
	buf[5] = f + 0x9B05688CU;
	buf[6] = g + 0x1F83D9ABU;
	buf[7] = h + 0x5BE0CD19U;
}

#undef RR
#undef SCHED
#undef ROUND

#endif
