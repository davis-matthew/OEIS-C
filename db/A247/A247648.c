/*
 * A247648  Numbers whose binary expansion begins and ends with 1 and does not
 *          contain two adjacent zeros.
 * https://oeis.org/A247648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer whose binary expansion starts 
 * and ends with 1 and does not contain two consecutive zeros. 
 * The sequence starts: 1,3,5,7,11,13,15,21,23,27,29,31,43,45,47,53,55,59,61,63,85,87,91,93,...
 * The algorithm first determines the length ℓ of the binary representation
 * using Fibonacci numbers, then constructs the ℓ‑bit string by a combinatorial
 * enumeration that respects the “no two consecutive zeros” constraint.
 * Complexity is O(ℓ) per call (ℓ ≈ logφ n), well below the limits.
 */
#include <stddef.h> /* for size_t */

#define MAX_LEN 1024 /* sufficient for all practical n (ℓ grows < 1000) */

static seq_t _dp[MAX_LEN][2];

/* Return the n-th term of sequence A247648 (1‑based). */
seq_t a(idx_t n)
{
    /* Find the minimal ℓ such that Fib_{ℓ+2} ≥ n+1. */

    seq_t target = (seq_t)n + 1;          /* target = n + 1 */
    seq_t f1 = 1;                         /* Fib_1 */
    seq_t f2 = 1;                         /* Fib_2 */
    idx_t m = 2;                          /* current index of f2 */

    while (f2 < target) {                 /* compute next Fibonacci until enough */
        seq_t f_next = f1 + f2;
        f1 = f2;
        f2 = f_next;
        ++m;                              /* f2 now Fib_m */
    }

    /* ℓ = m - 2  (since m = ℓ+2) */
    unsigned int len = (unsigned int)(m - 2);

    /* f1 now holds Fib_{ℓ+1} = Fib_{m-1} */
    seq_t prev_cum = f1;                  /* previous cumulative count +1 */
    seq_t offset = (seq_t)n - prev_cum;   /* 0‑based index within this length */

    /* Special case: length 1 => only the number 1 */
    if (len == 1) {
        return (seq_t)1;
    }

    /* Build DP for suffix counts: dp[pos][prev] = number of ways to complete
     * positions pos..len when previous bit is 'prev' (0 or 1).  Positions
     * are numbered 1..len; we compute only for pos >= 2.  pos==len has a
     * forced bit 1, so dp[len][*] = 1. */
    for (int pos = (int)len; pos >= 2; --pos) {
        if (pos == (int)len) {
            _dp[pos][0] = (seq_t)1;
            _dp[pos][1] = (seq_t)1;
        } else {
            _dp[pos][0] = _dp[pos + 1][1];                    /* prev 0 -> must place 1 */
            _dp[pos][1] = _dp[pos + 1][0] + _dp[pos + 1][1];  /* prev 1 -> 0 or 1 */
        }
    }

    /* Construct the ℓ‑bit number.  Bit 1 is fixed 1, bit ℓ is fixed 1.
     * For positions 2..ℓ-1 we decide 0 or 1 using the precomputed counts. */
    seq_t value = (seq_t)1;   /* first bit */
    int prev = 1;             /* previous bit at position 1 */

    for (unsigned int pos = 2; pos < len; ++pos) {
        seq_t cnt0 = 0;
        if (prev == 1) {               /* we can place 0 only if previous is 1 */
            cnt0 = _dp[pos + 1][0];
        }

        if (offset < cnt0) {           /* choose 0 */
            value = (value << 1);
            prev = 0;
        } else {                       /* choose 1 */
            offset -= cnt0;
            value = (value << 1) | (seq_t)1;
            prev = 1;
        }
    }

    /* Append the mandatory last bit '1'. */
    value = (value << 1) | (seq_t)1;

    return value;
}
