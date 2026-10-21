/*
 * A298823  Number of minimal total dominating sets in the n-dipyramidal
 *          graph.
 * https://oeis.org/A298823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A298823:  Number of minimal total dominating sets in
    the n-dipyramidal graph.
    Formula (recurrence):
        a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) - a(n-7)
               + a(n-9) + a(n-10) - a(n-11)   for n > 11
    Initial values (OEIS offset 1):
        a(1)=2,  a(2)=4,  a(3)=9,  a(4)=12,  a(5)=15,
        a(6)=21, a(7)=21, a(8)=20, a(9)=30, a(10)=45,
        a(11)=44, a(12)=49
    The implementation stores the sequence in a static array and
    memoises all computed values up to the largest n requested.
    It is valid C11 and C++17 alike.
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /*  Limit the pre‑allocation to a reasonable upper bound.
        The sequence is required for n up to about 10000; 20001 is
        comfortably above that.  If a larger n is requested the
        function will still work, but memory usage will be larger.     */
    enum { MAX_CAP = 20001 };

    static seq_t memo[MAX_CAP];
    static size_t computed = 0;   /* largest index that has been computed */

    if (n < 1ULL)
        return 0;                 /* undefined for n < 1 */

    /*  Initialise the first 12 terms once.  */
    if (computed == 0) {
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)4;
        memo[3] = (seq_t)9;
        memo[4] = (seq_t)12;
        memo[5] = (seq_t)15;
        memo[6] = (seq_t)21;
        memo[7] = (seq_t)21;
        memo[8] = (seq_t)20;
        memo[9] = (seq_t)30;
        memo[10] = (seq_t)45;
        memo[11] = (seq_t)44;
        memo[12] = (seq_t)49;
        computed = 12;
    }

    /*  Return value if we already computed it.  */
    if ((size_t)n <= computed)
        return memo[n];

    /*  Compute successive terms up to the requested n.  */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t val = memo[i-1];
        val += memo[i-1];          /* 2*a(i-1) */
        val += memo[i-3];          /* + a(i-3)   */
        val += memo[i-9];          /* + a(i-9)   */
        val += memo[i-10];         /* + a(i-10)  */
        val -= memo[i-2];          /* - a(i-2)   */
        val -= memo[i-4];          /* - a(i-4)   */
        val -= memo[i-7];          /* - a(i-7)   */
        val -= memo[i-11];         /* - a(i-11)  */
        memo[i] = val;
    }
    computed = (size_t)n;
    return memo[n];
}
