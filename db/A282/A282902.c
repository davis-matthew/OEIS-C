/*
 * A282902  Square array A(n, k) read by antidiagonals downwards:
 *          multiplicative order of 2 modulo prime(n)^k, where k runs over the
 *          positive integers.
 * https://oeis.org/A282902
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = multiplicative order of 2 modulo prime(n-1)^k, * read by
 *            antidiagonals downwards
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 82, u64 n <= 216,
 *            u128 n <= 604, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, mod_pow, mul_small,
 *            pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

static bool primes_init = false;
static uint64_t primes_arr[300];
static size_t primes_cnt;

/* multiplicative order of 2 modulo an odd prime p */
static seq_t order_mod_p(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t r = phi;
    for (size_t fi = 0; fi < primes_cnt && primes_arr[fi] * primes_arr[fi] <= r; ++fi) {
        uint64_t q = primes_arr[fi];
        while (r % q == 0) {
            seq_t v = mod_pow((seq_t)2, r / q, (seq_t)p);
            if (v == (seq_t)1)
                r /= q;
            else
                break;
        }
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* undefined for n < 2 */

    /* prime table once */
    if (!primes_init) {
        size_t cnt;
        uint64_t *pr;
        build_primes_upto(2000, &pr, &cnt);
        primes_cnt = cnt > 300 ? 300 : cnt;
        for (size_t i = 0; i < primes_cnt; ++i)
            primes_arr[i] = pr[i];
        primes_init = true;
    }

    /* zero‑based index within the infinite array */
    uint64_t m = n - 2;

    /* find diagonal d : (d-1)d/2 > m */
    uint64_t t = 1 + 8 * m;
    uint64_t sqrt_t = _floor_isqrt_u64(t);
    uint64_t d = (1 + sqrt_t) / 2 + 1;
    while ((d - 1) * d / 2 <= m) ++d;
    while ((d - 2) * (d - 1) / 2 > m) --d;

    /* position inside the diagonal */
    uint64_t prev = (d - 2) * (d - 1) / 2;
    uint64_t k = m + 1 - prev;    /* 1‑based row index */
    uint64_t i = k;                /* row index */
    uint64_t j = d - k;            /* column index (kCol) */

    /* prime for this row */
    uint64_t p_val;
    if (i == 1)
        p_val = 2;
    else
        p_val = primes_arr[i];    /* primes_arr is 0‑based */

    /* special case p = 2 : order is 2·3^(k-1) */
    if (p_val == 2) {
        seq_t pow3 = pow_seq((seq_t)3, (int)(j - 1));
        return mul_small(pow3, 2);
    }

    /* ordinary odd prime */
    seq_t r = order_mod_p(p_val);     /* order modulo p */
    uint64_t r_uint = (uint64_t)r;

    /* exponent s : largest t with p^t | (2^r - 1) */
    uint64_t s = 1;
    seq_t modulus = mul_small((seq_t)p_val, (unsigned)p_val); /* p^2 */
    while (true) {
        seq_t val = mod_pow((seq_t)2, r_uint, modulus);
        if (val == (seq_t)1) {
            ++s;
            modulus = mul_small(modulus, (unsigned)p_val);
        } else
            break;
    }

    /* compute answer */
    if (j == 1)
        return r;
    if ((uint64_t)j <= s)
        return r;

    uint64_t exp_needed = j - s;
    seq_t p_pow = pow_seq((seq_t)p_val, (int)exp_needed);
    return seq_mul(r, p_pow);
}
