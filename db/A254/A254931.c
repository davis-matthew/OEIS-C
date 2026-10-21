/*
 * A254931  Fundamental positive solution y = y2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = A001132(n), n >= 1, (primes congruent
 *          to 1 or 7 mod 8).
 * https://oeis.org/A254931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the larger of the two positive y’s in the fundamental
 *            solutions of x² – 2 y² = p, where p = the n‑th prime congruent
 *            to 1 or 7 (mod 8). The algorithm uses modular square roots to
 *            compute the two seed solutions and then selects the two smallest
 *            positive y’s. The second one is returned. Comp...
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt, next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/powmod64.h"

static uint64_t sqrt_mod_prime(uint64_t a, uint64_t p)
{
    if (a == 0) return 0;
    if (p == 2) return a;
    /* Legendre symbol check – no root if not 1 */
    if (powmod64(a, (p - 1) / 2, p) != 1) return 0;

    /* Tonelli–Shanks */
    uint64_t q = p - 1;
    uint64_t s = 0;
    while ((q & 1ULL) == 0) {
        q >>= 1;
        ++s;
    }

    /* find a quadratic non‑residue z */
    uint64_t z = 2;
    while (powmod64(z, (p - 1) / 2, p) != p - 1) ++z;

    uint64_t c = powmod64(z, q, p);
    uint64_t x = powmod64(a, (q + 1) / 2, p);
    uint64_t t = powmod64(a, q, p);
    uint64_t m = s;

    while (t != 1) {
        uint64_t i = 1;
        uint64_t t2 = (t * t) % p;
        while (i < m) {
            if (t2 == 1) break;
            t2 = (t2 * t2) % p;
            ++i;
        }
        uint64_t b = powmod64(c, 1ULL << (m - i - 1), p);
        x = (x * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    return x;
}

static uint64_t sqrt2_mod_prime(uint64_t p)
{
    return sqrt_mod_prime(2ULL, p);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static uint64_t *prime_list = NULL;
    static size_t cap = 0;
    static size_t cnt = 0;
    static uint64_t last_checked = 1;

    /* build sufficient list of primes ≡ 1 or 7 (mod 8) */
    while (cnt < (size_t)n) {
        last_checked = next_prime(last_checked);
        if ((last_checked & 7ULL) == 1ULL || (last_checked & 7ULL) == 7ULL) {
            if (cnt == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                cap = new_cap;
            }
            prime_list[cnt++] = last_checked;
        }
    }

    uint64_t p = prime_list[n - 1];
    uint64_t r = sqrt2_mod_prime(p);      /* r² ≡ 2 (mod p) */
    if (r == 0) return 0;                 /* should not happen */

    unsigned __int128 r2_128 = (unsigned __int128)r * r;
    uint64_t s = (uint64_t)((r2_128 - 2) / p);   /* s ≥ 1 */

    /* collect the two smallest positive y’s */
    uint64_t cand[8];
    int cand_cnt = 0;

    for (uint64_t k = 0; ; ++k) {
        unsigned __int128 d128 = (unsigned __int128)2 * k * k + s;
        uint64_t d   = (uint64_t)d128;
        seq_t   dseq = (seq_t)d;
        seq_t   msq  = floor_sqrt(dseq);
        if (msq * msq != dseq) continue;           /* not a perfect square */
        uint64_t m = (uint64_t)msq;

        /* sign + */
        {
            int64_t num = -(int64_t)r * (int64_t)k + (int64_t)m;
            if (num > 0 && ((uint64_t)num % s == 0)) {
                uint64_t y = (uint64_t)num / s;
                int dup = 0;
                for (int i = 0; i < cand_cnt; ++i) if (cand[i] == y) { dup = 1; break; }
                if (!dup) cand[cand_cnt++] = y;
            }
        }
        {
            int64_t num = -(int64_t)r * (int64_t)k - (int64_t)m;
            if (num > 0 && ((uint64_t)num % s == 0)) {
                uint64_t y = (uint64_t)num / s;
                int dup = 0;
                for (int i = 0; i < cand_cnt; ++i) if (cand[i] == y) { dup = 1; break; }
                if (!dup) cand[cand_cnt++] = y;
            }
        }
        /* sign – */
        {
            int64_t num = (int64_t)r * (int64_t)k + (int64_t)m;
            if (num > 0 && ((uint64_t)num % s == 0)) {
                uint64_t y = (uint64_t)num / s;
                int dup = 0;
                for (int i = 0; i < cand_cnt; ++i) if (cand[i] == y) { dup = 1; break; }
                if (!dup) cand[cand_cnt++] = y;
            }
        }
        {
            int64_t num = (int64_t)r * (int64_t)k - (int64_t)m;
            if (num > 0 && ((uint64_t)num % s == 0)) {
                uint64_t y = (uint64_t)num / s;
                int dup = 0;
                for (int i = 0; i < cand_cnt; ++i) if (cand[i] == y) { dup = 1; break; }
                if (!dup) cand[cand_cnt++] = y;
            }
        }

        if (cand_cnt >= 2) break;
    }

    /* sort found y’s */
    for (int i = 0; i < cand_cnt - 1; ++i)
        for (int j = i + 1; j < cand_cnt; ++j)
            if (cand[i] > cand[j]) {
                uint64_t t = cand[i];
                cand[i] = cand[j];
                cand[j] = t;
            }

    return (seq_t)cand[1];   /* second smallest positive y */
}
