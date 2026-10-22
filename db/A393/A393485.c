/*
 * A393485  Areas of Heron triangles whose sides all have prime power lengths.
 * https://oeis.org/A393485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(1) = 6; for n >= 2, a(n) = 2*(A019434(n) - 2)*(A019434(n-1) -
 *            1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A019434(n) is the nth Fermat prime: 3, 5, 17, 257, 65537, ... */

seq_t a(idx_t n)
{
    /* First five Fermat primes */
    const seq_t fermat[5] = {
        (seq_t)3,
        (seq_t)5,
        (seq_t)17,
        (seq_t)257,
        (seq_t)65537
    };

    if (n == 1) {
        /* The first term: area of 3-4-5 triangle */
        return (seq_t)6;
    }

    if (n < 2 || n > 5) {
        /* No known Fermat primes beyond the 5th, hence sequence undefined */
        return (seq_t)0;
    }

    /* For 2 <= n <= 5 */
    idx_t idx = n - 1;                      /* 1‑based index in fermat array */
    seq_t fn   = fermat[idx];               /* A019434(n)   */
    seq_t fn_1 = fermat[idx - 1];           /* A019434(n-1) */

    return (seq_t)2 * (fn - (seq_t)2) * (fn_1 - (seq_t)1);
}
