/*
 * A254732  a(n) is the least k > n such that n divides k^2.
 * https://oeis.org/A254732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > n such that n divides k^2
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: integer square root for unsigned 64-bit */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    idx_t bit = 1ULL << 62; /* highest power of four <= UINT64_MAX */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Compute k0 = product over primes p | n of p^ceil(e/2) where e = v_p(n) */
    idx_t temp_n = n;
    seq_t k0 = 1;            /* start with multiplicative identity */
    /* handle factor 2 separately for speed */
    if (temp_n % 2 == 0) {
        idx_t cnt = 0;
        while (temp_n % 2 == 0) {
            temp_n /= 2;
            cnt++;
        }
        idx_t r = (cnt + 1) / 2;         /* ceil(cnt/2) */
        for (idx_t i = 0; i < r; ++i) k0 *= (seq_t)2;
    }
    /* odd factors */
    idx_t limit = isqrt_u64(temp_n);
    for (idx_t p = 3; p <= limit; p += 2) {
        if (temp_n % p == 0) {
            idx_t cnt = 0;
            while (temp_n % p == 0) {
                temp_n /= p;
                cnt++;
            }
            idx_t r = (cnt + 1) / 2;
            seq_t pp = 1;
            for (idx_t i = 0; i < r; ++i) pp *= (seq_t)p;
            k0 *= pp;
            limit = isqrt_u64(temp_n); /* recompute bound if n shrinks */
        }
    }
    /* remaining prime factor > sqrt(original) */
    if (temp_n > 1) {
        /* its exponent is 1, so r = ceil(1/2) = 1 */
        k0 *= (seq_t)temp_n;
    }

    /* Now compute a(n) = k0 * (n / k0 + 1) with strict inequality */
    seq_t sn = (seq_t)n;
    seq_t m = sn / k0;          /* floor division */
    return k0 * (m + 1);
}
