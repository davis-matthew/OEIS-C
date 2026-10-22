/*
 * A361291  a(n) = ((2*n + 1)^n - 1)/(2*n).
 * https://oeis.org/A361291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n + 1)^n - 1)/(2*n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 by the sequence specification; handle 0 idempotently */
    if (n == 0)
        return (seq_t)0;

    /* For integral seq_t types of 8 bytes or less we avoid
       overflow of (2*n+1)^n by using 128‑bit temporaries. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit arithmetic (available on GCC/Clang; defined
           for C11 and C++17) */
        unsigned __int128 k = (unsigned __int128)2 * n + 1;
        unsigned __int128 sum  = 0;
        unsigned __int128 power = 1;

        for (idx_t i = 0; i < n; ++i) {
            sum += power;        /* accumulate 1 + k + k^2 + ...   */
            power *= k;          /* next power: k^(i+1)            */
        }
        return (seq_t)sum;       /* cast back to seq_t (fits here)   */
    }

    /* For larger seq_t types (e.g. big integers) we use the
       provided big_pow routine and ordinary division. */
    seq_t k   = (seq_t)n * 2 + 1;                /* 2*n + 1          */
    seq_t pow_k_n = big_pow(k, (seq_t)n);        /* k^n              */
    seq_t num   = pow_k_n - 1;                   /* k^n - 1          */
    seq_t den   = (seq_t)n * 2;                  /* 2*n              */
    return num / den;                            /* exact division    */
}
