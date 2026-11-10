/*
 * A322869  a(n) = A000120(A048675(n)).
 * https://oeis.org/A322869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(A048675(n)); A048675(n) = Sum_{p^e || n}
 *            e*2^(PrimePi(p)-1); a(n) = popcount of that sum (computed with a
 *            bit array and carries)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9060 (2026-11-08)
 * Helpers:   mem, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/prime_pi.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    uint64_t t = (uint64_t)n;

    for (uint64_t d = 2; d <= t / d; d += (d == 2 ? 1 : 2)) {
        if (t % d == 0) {
            uint64_t e = 0;
            while (t % d == 0) { t /= d; e++; }
            pr[cnt] = d; ex[cnt] = e; cnt++;
        }
    }
    if (t > 1) { pr[cnt] = t; ex[cnt] = 1; cnt++; }

    size_t maxpos = 0;
    size_t pos[64];
    for (size_t i = 0; i < cnt; ++i) {
        pos[i] = (size_t)(long long)prime_pi((idx_t)pr[i]) - 1;
        if (pos[i] > maxpos) maxpos = pos[i];
    }

    size_t size = maxpos + 80;
    unsigned char *bits = mem_new(unsigned char, size);

    for (size_t k = 0; k < cnt; ++k) {
        size_t i = pos[k];
        uint64_t carry = ex[k];
        while (carry) {
            uint64_t s = (uint64_t)bits[i] + (carry & 1);
            bits[i] = (unsigned char)(s & 1);
            carry = (carry >> 1) + (s >> 1);
            i++;
        }
    }

    seq_t c = 0;
    for (size_t i = 0; i < size; ++i) if (bits[i]) c++;
    mem_free(bits);
    return c;
}
