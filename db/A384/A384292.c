/*
 * A384292  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (214013*s+13523655) mod 2^24 when
 *          started at s=1.
 * https://oeis.org/A384292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n) = (214013*a(n-1)+13523655) mod 2^24
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A384292, with offset 1.  The function
   is resilient to calls in any order and has O(n) running time,
   which easily satisfies the time constraints for n up to about
   10 000.  No caching or auxiliary data structures are used. */
seq_t a(idx_t n)
{
    /* If the index is 0 (outside the defined offset), return 0. */
    if (n == 0) return (seq_t)0;

    const seq_t mult = (seq_t)214013;
    const seq_t add  = (seq_t)13523655;
    const seq_t mask = (seq_t)0xFFFFFF;   /* 2^24 - 1 */

    seq_t s = (seq_t)1;          /* a(1) */
    idx_t i;

    for (i = 1; i < n; ++i) {
        s = (mult * s + add) & mask;
    }
    return s;
}
