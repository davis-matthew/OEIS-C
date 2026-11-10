/*
 * A342017  a(n) = A342007(A327860(n)), where A342007 is multiplicative with
 *          a(p^e) = p^floor(e/p), and A327860 is arithmetic derivative of
 *          A276086.
 * https://oeis.org/A342017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A342007(A003415(A276086(n))), where A342007 is
 *            multiplicative with a(p^e) = p^floor(e/p). A276086(n) = Prod
 *            prime(k+1)^d_k (d_k the primorial-base digits of n); its
 *            derivative is m * Sum d_k/prime(k+1)
 * Range:     u8 n <= 19, u16 n <= 86, u32 n <= 6799, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9864 (2026-11-10)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static bool A342017_small_prime(unsigned p) {
    if (p < 2) return false;
    for (unsigned i = 2; i * i <= p; i++)
        if (p % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* digits in primorial base */
    unsigned digs[32];
    unsigned pr[32];
    int cnt = 0;
    uint64_t x = (uint64_t)n;
    unsigned p = 2;
    while (x > 0 && cnt < 32) {
        digs[cnt] = (unsigned)(x % p);
        pr[cnt] = p;
        x /= p;
        cnt++;
        do { p++; } while (!A342017_small_prime(p));
    }

    seq_t m = 1;
    for (int k = 0; k < cnt; k++)
        for (unsigned j = 0; j < digs[k]; j++)
            m *= (seq_t)pr[k];

    /* arithmetic derivative: sum d_k * m / p_k */
    seq_t D = 0;
    for (int k = 0; k < cnt; k++)
        if (digs[k] > 0)
            D += (m / (seq_t)pr[k]) * (seq_t)digs[k];

    if (D == 0) return 1;

    seq_t result = 1;
    for (unsigned q = 2; q < 200; q++) {
        if (!A342017_small_prime(q)) continue;
        /* stop when q^q > D */
        seq_t t = 1;
        bool exceed = false;
        for (unsigned i = 0; i < q; i++) {
            if (t > D / (seq_t)q) { exceed = true; break; }
            t *= (seq_t)q;
        }
        if (exceed) break;

        unsigned e = 0;
        while (D % (seq_t)q == 0) {
            D /= (seq_t)q;
            e++;
        }
        unsigned f = e / q;
        for (unsigned i = 0; i < f; i++)
            result *= (seq_t)q;
    }
    return result;
}
