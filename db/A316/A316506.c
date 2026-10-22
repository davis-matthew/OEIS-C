/*
 * A316506  a(n) is the rank of the multiplicative group of Gaussian integers
 *          modulo n.
 * https://oeis.org/A316506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes rank of the units of the Gaussian integers modulo n
   (OEIS A316506).  The group is expressed as a product of cyclic
   factors whose orders are determined by the prime‐factorisation of
   n.  The rank equals the maximum, over all primes q, of the
   number of cyclic components divisible by q. */
#include <stdint.h>
#include <stddef.h>

static seq_t pow_uint_u64(uint64_t base, uint64_t exp) {
    seq_t res = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) res *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t nn = n;
    uint64_t prime_n[64];
    uint8_t   exp_n[64];
    size_t    cnt_n = 0;

    uint64_t tmp = nn;
    if (tmp % 2 == 0) {
        uint8_t e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        prime_n[cnt_n] = 2; exp_n[cnt_n++] = e;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            prime_n[cnt_n] = p; exp_n[cnt_n++] = e;
        }
    }
    if (tmp > 1) {
        prime_n[cnt_n] = tmp; exp_n[cnt_n++] = 1;
    }

    seq_t orders[64];
    size_t cnt_orders = 0;

    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = prime_n[i];
        uint8_t e = exp_n[i];
        if (p == 2) {
            if (e == 1) {
                orders[cnt_orders++] = (seq_t)2;
            } else if (e == 2) {
                orders[cnt_orders++] = (seq_t)4;
                orders[cnt_orders++] = (seq_t)2;
            } else {
                orders[cnt_orders++] = (seq_t)4;
                orders[cnt_orders++] = (seq_t)(1ULL << (e - 2));
                orders[cnt_orders++] = (seq_t)(1ULL << (e - 1));
            }
        } else if ((p & 3ULL) == 1ULL) {            /* p ≡ 1 (mod 4) */
            seq_t pow_part = pow_uint_u64(p, e - 1);
            seq_t order    = ((seq_t)p - 1) * pow_part;
            orders[cnt_orders++] = order;
            orders[cnt_orders++] = order;
        } else {                                   /* p ≡ 3 (mod 4) */
            if (e == 1) {
                seq_t order = (seq_t)p * (seq_t)p - 1;
                orders[cnt_orders++] = order;
            } else {
                seq_t pow_part = pow_uint_u64(p, e - 1);
                seq_t order1   = pow_part;
                seq_t order2   = pow_part * (((seq_t)p * (seq_t)p) - 1);
                orders[cnt_orders++] = order1;
                orders[cnt_orders++] = order2;
            }
        }
    }

    /* Count, for each prime, how many cyclic components are divisible by it. */
    uint64_t prime_list[64];
    size_t   prime_counts[64];
    size_t   prime_len = 0;

    for (size_t i = 0; i < cnt_orders; ++i) {
        seq_t val128 = orders[i];
        uint64_t v = (uint64_t)val128;           /* safe for n ≤ 10000 */
        /* factor v into distinct primes */
        for (uint64_t p = 2; p * p <= v; ++p) {
            if (v % p == 0) {
                /* record p once for this component */
                size_t j;
                for (j = 0; j < prime_len; ++j)
                    if (prime_list[j] == p) break;
                if (j == prime_len) {
                    prime_list[prime_len] = p;
                    prime_counts[prime_len] = 1;
                    ++prime_len;
                } else {
                    ++prime_counts[j];
                }
                while (v % p == 0) v /= p;
            }
        }
        if (v > 1) {                              /* remaining prime factor */
            uint64_t p = v;
            size_t j;
            for (j = 0; j < prime_len; ++j)
                if (prime_list[j] == p) break;
            if (j == prime_len) {
                prime_list[prime_len] = p;
                prime_counts[prime_len] = 1;
                ++prime_len;
            } else {
                ++prime_counts[j];
            }
        }
    }

    seq_t rank = 0;
    for (size_t i = 0; i < prime_len; ++i)
        if (prime_counts[i] > rank) rank = prime_counts[i];
    return rank;
}
