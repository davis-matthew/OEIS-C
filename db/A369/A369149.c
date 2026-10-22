/*
 * A369149  a(n) is the size of the largest subset of {1,...,n} such that no
 *          two elements differ by 4 or 7.
 * https://oeis.org/A369149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=4, a(6)=4, a(7)=4, a(8)=4,
 *            a(9)=5, a(10)=5, a(11)=5, a(12)=6, a(13)=7, a(14)=7, a(15)=8,
 *            a(16)=8. For n ≥ 17: a(n) = a(n-11) + 5. (Equivalent to
 *            a(n+11)=a(n)+5 for n>5.)
 * Range:     u8 n <= 561, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A369149. */
seq_t a(idx_t n)
{
    /* Base values for n = 1 .. 16.  */
    static const seq_t base[16] = {
        1ULL, 2ULL, 3ULL, 4ULL, 4ULL, 4ULL, 4ULL, 4ULL,
        5ULL, 5ULL, 5ULL, 6ULL, 7ULL, 7ULL, 8ULL, 8ULL
    };

    if (n <= 16) {
        return base[n - 1];
    }

    /* For n ≥ 17, use the recurrence a(n) = a(n-11)+5.
       We compute how many 11‑step jumps are needed. */
    idx_t jumps = (n - 6) / 11;          /* number of times 5 is added   */
    idx_t pos   = n - jumps * 11;        /* position inside the base block (6 .. 16) */

    /* pos is guaranteed to be between 6 and 16 inclusive. */
    seq_t result = base[pos - 1] + (seq_t)jumps * 5ULL;
    return result;
}
