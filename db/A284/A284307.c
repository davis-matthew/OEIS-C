/*
 * A284307  Permutation of the natural numbers partitioned into quadruples
 *          [4k-3, 4k, 4k-2, 4k-1], k > 0.
 * https://oeis.org/A284307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 4 for n > 4, with a(1)=1, a(2)=4, a(3)=2, a(4)=3
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent non-recursive form: a(n) = 4*floor((n-1)/4) + base[n mod 4] */
/* where base = {3,1,4,2} for remainders {0,1,2,3} respectively. */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – undefined otherwise */
        return 0;

    /* base values for remainder n % 4: 0->3, 1->1, 2->4, 3->2 */
    const seq_t base[4] = { 3, 1, 4, 2 };

    idx_t times = (n - 1) / 4;  /* number of complete 4-element blocks before n */
    seq_t result = ((seq_t)times) * 4 + base[n % 4];
    return result;
}
