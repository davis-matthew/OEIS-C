/*
 * A272902  Numbers k such that 6*k - 5 is not prime.
 * https://oeis.org/A272902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth integer k such that 6*k − 5 is NOT prime.  */

#include <stdbool.h>

/* Check whether an unsigned 64‑bit integer is composite or 1.  */
static inline bool isComposite(idx_t v)
{
    if (v < 2)                /* 1 and 0 are not prime */
        return true;
    if (v % 2 == 0)           /* even number: only 2 is prime */
        return v != 2;
    for (idx_t i = 3; i <= v / i; i += 2) {
        if (v % i == 0)
            return true;      /* found a proper divisor */
    }
    return false;             /* no non‑trivial divisor found */
}

/*  Return the nth element of the sequence,  n ≥ 1.  */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many qualifying k values found so far */
    idx_t k     = 0;      /* candidate integer k */

    while (1) {
        ++k;                         /* next k to test */
        idx_t val = 6 * k - 5;        /* number whose primality matters */

        if (isComposite(val)) {       /* 6*k-5 is not prime (or 1) */
            ++count;
            if (count == n)           /* desired term reached */
                return (seq_t)k;     /* return k as sequence element */
        }
    }
}
