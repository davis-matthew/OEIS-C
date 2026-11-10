/*
 * A328579  a(n) = A053669(A276086(A276086(n))).
 * https://oeis.org/A328579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053669(A276086(A276086(n))); computed as the smallest
 *            prime whose digit is 0 in the primorial-base expansion of m =
 *            A276086(n)
 * Range:     u8 n <= 19, u16 n <= 85, u32 n <= 823, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 122 terms through n = 8961 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A328579_nth_prime(int k)
{
    /* k >= 0, returns prime(k+1) by trial division */
    uint32_t cnt = 0, c = 1;
    while (1) {
        c++;
        bool ok = true;
        for (uint32_t d = 2; (uint64_t)d * d <= c; d++)
            if (c % d == 0) { ok = false; break; }
        if (ok) {
            if ((int)cnt == k) return c;
            cnt++;
        }
    }
}

seq_t a(idx_t n)
{
    /* m = A276086(n) */
    seq_t m = 1;
    {
        idx_t rem = n;
        uint32_t p = 2;
        int k = 0;
        while (rem > 0) {
            idx_t d = rem % p;
            for (idx_t j = 0; j < d; j++) m = m * (seq_t)p;
            rem /= p;
            k++;
            p = A328579_nth_prime(k);
        }
    }

    /* primorial-base digits of m, record positions with nonzero digit */
    static bool used[1024];
    for (int i = 0; i < 1024; i++) used[i] = false;
    seq_t rem = m;
    int k = 0;
    uint32_t p = 2;
    while (rem > 0 && k < 1023) {
        seq_t d = rem % (seq_t)p;
        if (d != 0) used[k] = true;
        rem = rem / (seq_t)p;
        k++;
        p = A328579_nth_prime(k);
    }
    int j = 0;
    while (used[j]) j++;
    return (seq_t)A328579_nth_prime(j);
}
