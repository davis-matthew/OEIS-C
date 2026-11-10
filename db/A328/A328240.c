/*
 * A328240  Numbers k such that the second arithmetic derivative of A276086(k)
 *          is prime.
 * https://oeis.org/A328240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003415(A003415(A276086(k))) prime;
 *            search over k. With v = A276086(k) = Prod p_i^d_i: v' = g*T, g =
 *            Prod_{d_i>=2} p_i^(d_i-1), T = Sum d_i*r/p_i, r = rad(v). v''
 *            can be prime only if v' is squarefree (q^2 | x implies q | x',
 *            and x' > q), so every d_i <= 2. Then v'' = Sum_...
 * Range:     u8 n <= 20, u16 n <= 190, u32 n <= 1217, u64 n <= 1217,
 *            u128 n <= 1217, big any n
 * Validated: standard, pass, 77 terms through n = 1180 (2026-11-08)
 * Helpers:   is_prime_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u128.h"

static const uint64_t A328240_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static bool A328240_test(idx_t k)
{
    int d[16];
    int cnt = 0;
    idx_t rem = k;
    while (rem > 0 && cnt < 16) {
        d[cnt] = (int)(rem % A328240_pr[cnt]);
        rem /= A328240_pr[cnt];
        if (d[cnt] > 2) return false;           /* p^2 | v' -> v'' not prime */
        cnt++;
    }
    unsigned __int128 r = 1, g = 1;
    for (int i = 0; i < cnt; i++) {
        if (d[i] > 0) r *= A328240_pr[i];
        if (d[i] == 2) g *= A328240_pr[i];
    }
    unsigned __int128 T = 0;
    for (int i = 0; i < cnt; i++)
        if (d[i] > 0) T += (r / A328240_pr[i]) * (unsigned)d[i];
    unsigned __int128 x = g * T;                /* x = v' */
    if (x < 2) return false;
    /* g and T must be coprime (g squarefree by construction) */
    for (int i = 0; i < cnt; i++)
        if (d[i] == 2 && T % A328240_pr[i] == 0) return false;
    unsigned __int128 s = 0;                    /* s = x' */
    for (int i = 0; i < cnt; i++)
        if (d[i] == 2) s += x / A328240_pr[i];
    /* factor T, requiring squarefree */
    unsigned __int128 t = T;
    for (uint64_t q = 2; (unsigned __int128)q * q <= t; q += (q == 2 ? 1 : 2)) {
        if (t % q == 0) {
            t /= q;
            if (t % q == 0) return false;
            s += x / q;
        }
    }
    if (t > 1) s += x / t;
    if (s < 2) return false;
    return is_prime_u128(s);
}

seq_t a(idx_t n)
{
    static idx_t *terms = NULL;
    static size_t have = 0, cap = 0;
    static idx_t k = 0;
    while (have < (size_t)n) {
        k++;
        if (A328240_test(k)) {
            if (have == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (idx_t *)realloc(terms, cap * sizeof(idx_t));
                if (!terms) abort();
            }
            terms[have++] = k;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
