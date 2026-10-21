/*
 * A295989  Irregular triangle T(n, k), read by rows, n >= 0 and 0 <= k <
 *          A001316(n): T(n, k) is the (k+1)-th nonnegative number m such that
 *          n AND m = m (where AND denotes the bitwise AND operator).
 * https://oeis.org/A295989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   a(n) is the n‑th element of the flattened irregular triangle T,
 *            where row r consists of all submasks of r in increasing numeric
 *            order. Implementation finds the row containing n, then returns
 *            the k‑th submask by linear scan. This is O(r) with r ≤ n and is
 *            fast for n up to 10 000
 * Range:     u8 n <= 6561, u16 n <= 6562, u32 n <= 6562, u64 n <= 6562,
 *            u128 n <= 6562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Popcount for idx_t (unsigned 64‑bit).  Falls back to a portable
   implementation if the compiler has no builtin. */
static inline int popcnt(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    /* __builtin_popcountll works with unsigned long long.  idx_t is
       guaranteed to be a 64‑bit unsigned type. */
    return __builtin_popcountll((unsigned long long)x);
#else
    int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/* Size of the row r: 2^(popcount(r)).  For safety we limit shifts to
   under 63 bits (r ≤ 10000 ⇒ popcount(r) ≤ 14). */
static inline idx_t row_size(idx_t r)
{
    int p = popcnt(r);
    /* Use a 64‑bit shift; the result fits in idx_t. */
    return ((idx_t)1) << p;
}

/* Return the n‑th term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Find the row that contains index n.  cum holds the total number
       of elements in all preceding rows. */
    idx_t cum = 0;
    idx_t r   = 0;
    while (1) {
        idx_t sz = row_size(r);
        if (cum + sz > n) /* n falls in this row */
            break;
        cum += sz;
        ++r;
    }

    /* Index of the term within row r. */
    idx_t k = n - cum;  /* 0‑based */

    /* Linear scan of submasks of r to reach the k‑th one. */
    idx_t count = 0;
    for (idx_t m = 0; m <= r; ++m) {
        if ((m & r) == m) {   /* m is a submask of r */
            if (count == k)
                return (seq_t)m;
            ++count;
        }
    }

    /* Should never reach here. */
    return (seq_t)0;
}
