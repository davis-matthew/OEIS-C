/*
 * A348502  a(n) = A348498(A276086(n)).
 * https://oeis.org/A348502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A348498(A276086(n)); with m = prod p_i^E_i given directly
 *            by the primorial-base digits of n, a(n) = (sum_i E_i *
 *            rad(m)/p_i) / (2 if every E_i is even else 1), a(0) = 0
 * Range:     u8 n <= 40, u16 n <= 2578, u32 n <= 11550, u64 n <= 11550,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 95 terms through n = 7776 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A348502_small_prime(int k)
{
    /* k-th prime, k >= 1, by trial division (k is tiny) */
    int cnt = 0;
    uint64_t c = 1;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t pr[32];
    uint64_t ex[32];
    int cnt = 0;
    uint64_t rem = (uint64_t)n;
    int k = 1;
    bool all_even = true;

    while (rem > 0) {
        uint64_t p = A348502_small_prime(k);
        uint64_t d = rem % p;
        rem /= p;
        if (d > 0) {
            pr[cnt] = p;
            ex[cnt] = d;
            cnt++;
            if (d & 1ULL) all_even = false;
        }
        k++;
    }

    seq_t rad = 1;
    for (int i = 0; i < cnt; i++) rad *= (seq_t)pr[i];

    seq_t sum = 0;
    for (int i = 0; i < cnt; i++) {
        sum += (seq_t)ex[i] * (rad / (seq_t)pr[i]);
    }
    if (all_even) sum /= 2;
    return sum;
}
