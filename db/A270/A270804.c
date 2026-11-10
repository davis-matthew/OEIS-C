/*
 * A270804  0 followed by the positions of the 1's in the inverse Thue-Morse
 *          sequence A270803.
 * https://oeis.org/A270804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for k>=1, a(k) = position of the k-th 1 in A270803,
 *            where A270803: b(0)=0, b(1)=b(2)=1, b(3)=0; for m>=4: m%4==0:
 *            b(m)=b(m-1); m%4==1: b(m)=b(m-2); m%4==2: b(m)=b(m-3); m%4==3:
 *            b(m)=(b(m-4)+b((m-3)/4)) mod 2
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A270804_bits = NULL;
static size_t A270804_len = 0;
static size_t A270804_cap = 0;
static uint64_t *A270804_pos = NULL;
static size_t A270804_cnt = 0;
static size_t A270804_poscap = 0;

static void A270804_push(uint64_t p)
{
    if (A270804_cnt >= A270804_poscap) {
        size_t nc = A270804_poscap ? A270804_poscap * 2 : 64;
        A270804_pos = (uint64_t *)realloc(A270804_pos, nc * sizeof(uint64_t));
        A270804_poscap = nc;
    }
    A270804_pos[A270804_cnt++] = p;
}

static void A270804_extend(size_t newlen)
{
    if (newlen < 4) newlen = 4;
    if (newlen > A270804_cap) {
        A270804_bits = (unsigned char *)realloc(A270804_bits, newlen);
        A270804_cap = newlen;
    }
    size_t i = A270804_len;
    if (i < 4) {
        A270804_bits[0] = 0; A270804_bits[1] = 1;
        A270804_bits[2] = 1; A270804_bits[3] = 0;
        A270804_push(1);
        A270804_push(2);
        i = 4;
    }
    for (; i < newlen; ++i) {
        unsigned char v = 0;
        switch (i & 3u) {
            case 0: v = A270804_bits[i - 1]; break;
            case 1: v = A270804_bits[i - 2]; break;
            case 2: v = A270804_bits[i - 3]; break;
            default: v = (A270804_bits[i - 4] + A270804_bits[(i - 3) / 4]) & 1; break;
        }
        A270804_bits[i] = v;
        if (v) A270804_push((uint64_t)i);
    }
    A270804_len = newlen;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A270804_cnt < (size_t)n) {
        size_t nl = A270804_len ? A270804_len * 2 : 256;
        A270804_extend(nl);
    }
    return (seq_t)A270804_pos[(size_t)n - 1];
}
