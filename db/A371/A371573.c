/*
 * A371573  a(n) = lcm(A020639(n), A032742(n)).
 * https://oeis.org/A371573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula (OEIS A371573): a(n) = n if the smallest prime factor p of n
 * does not satisfy p^2 | n; otherwise a(n) = n / p.
 * The sequence also satisfies a(1) = 1 and a(p) = p for prime p.
 */
#include <stdint.h>

static idx_t _spf(idx_t n) {
    if (n < 2)
        return n;
    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0)
            return i;
    }
    return n;                 /* n is prime */
}

seq_t a(idx_t n) {
    /* Handle offset 1: a(1)=1; behaviour for n<1 is irrelevant */
    if (n <= 1)
        return (seq_t)1;

    idx_t p = _spf(n);          /* smallest prime factor */
    seq_t n_seq = (seq_t)n;
    seq_t p_seq = (seq_t)p;

    seq_t div = n_seq / p_seq;  /* n / p */
    if (div % p_seq == 0)       /* check if p^2 | n */
        return div;             /* n / p */
    else
        return n_seq;           /* n */
}
