/*
 * A323511  a(n) = 1 if n is squarefree and A079559(A048675(n)) = 1, otherwise
 *          a(n) = 0.
 * https://oeis.org/A323511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008966(n) * A079559(A048675(n)). For squarefree n, m =
 *            A048675(n) is the integer with bit (i-1) set for each prime(i) |
 *            n. A079559(m) = #{k : popcount(m+k) = k and m+k even}, since a
 *            set S of j>=1 gives sum(2^j-1) = (sum 2^j) - |S|. a(n) = 1 iff n
 *            is squarefree and that count is exactly 1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 131 terms through n = 9679 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* squarefree test and largest prime factor */
    idx_t temp = n;
    for (idx_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            temp /= i;
            if (temp % i == 0) return 0;
        }
    }
    idx_t P = (n == 1) ? 1 : temp; /* largest prime factor (temp is >1 whenever n>1) */
    /* if n>1, after loop temp is the largest prime factor, or it was reduced fully */
    if (n > 1) {
        /* recompute largest prime factor robustly */
        idx_t t2 = n, lp = 1;
        for (idx_t i = 2; i <= t2 / i; ++i) {
            while (t2 % i == 0) { t2 /= i; lp = i; }
        }
        if (t2 > 1) lp = t2;
        P = lp;
    }

    size_t L = 0;
    unsigned char *bits = 0;
    if (n > 1) {
        size_t sz = (size_t)P + 1;
        unsigned char *comp = (unsigned char *)calloc(sz, 1);
        if (!comp) return 0;
        /* count primes up to P to size bit array */
        size_t cnt = 0;
        for (size_t i = 2; i < sz; ++i) {
            if (!comp[i]) {
                ++cnt;
                for (size_t j = i * i; j < sz && j >= i; j += i) comp[j] = 1;
            }
        }
        L = cnt;
        bits = (unsigned char *)calloc(L + 1, 1);
        if (!bits) { free(comp); return 0; }
        size_t c = 0;
        for (size_t i = 2; i < sz; ++i) {
            if (!comp[i]) {
                ++c;
                if (n % (idx_t)i == 0) bits[c - 1] = 1;
            }
        }
        free(comp);
    }

    /* low 32 bits, popcount of high part, trailing ones of high part */
    uint64_t low = 0;
    uint64_t hp = 0;
    uint64_t t = 0;
    for (size_t i = 0; i < L; ++i) {
        if (bits[i]) {
            if (i < 32) low |= ((uint64_t)1 << i);
            else ++hp;
        }
    }
    for (size_t i = 32; i < L && bits[i]; ++i) ++t;
    if (bits) free(bits);

    uint64_t count = 0;
    for (uint64_t k = 0; k <= (uint64_t)L + 1; ++k) {
        uint64_t s = low + k;
        if (s & 1) continue;
        uint64_t carry = s >> 32;
        uint64_t ls = s & 0xffffffffULL;
        uint64_t pc = 0;
        while (ls) { ls &= ls - 1; ++pc; }
        pc += hp;
        if (carry) pc = pc + 1 - t;
        if (pc == k) {
            ++count;
            if (count > 1) return 0;
        }
    }
    return (seq_t)(count == 1 ? 1 : 0);
}
