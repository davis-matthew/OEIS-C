/*
 * A379269  Numbers whose binary representation has exactly three zeros.
 * https://oeis.org/A379269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose binary representation has exactly three zeros.
 *            a(n) is found by combinatorial consideration of binary strings
 *            of length L with leading 1 and exactly 3 zero bits among the
 *            remaining L−1 bits. For each n we locate the corresponding
 *            bit‑length L, unrank the 0‑positions, and construct t...
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t binom2(idx_t n) { return n * (n - 1) / 2; }
static idx_t binom3(idx_t n) { return n * (n - 1) * (n - 2) / 6; }
static idx_t binom4(idx_t n) { return n * (n - 1) * (n - 2) * (n - 3) / 24; }

/* Return the nth element (1‑based) of A379269. */
seq_t a(idx_t n)
{
    /* ---------- find the binary length L ----------
       C(L,4) gives the total count of numbers with
       at most L bits.  Find smallest L with C(L,4) >= n. */
    idx_t L = 4;
    while (binom4(L) < n) ++L;

    /* ---------- unrank within the block of length L ----------
       cumulative count up to length L-1 is C(L-1,4). */
    idx_t prev = binom4(L - 1);
    idx_t j = n - prev - 1;          /* 0‑based index inside this block */
    idx_t k = L - 1;                 /* number of lower bits */

    /* --------- find the three zero positions (i0,i1,i2) --------
     * The enumeration order is:
     *   for i0 from k-1 downto 2
     *     for i1 from i0-1 downto 1
     *       for i2 from i1-1 downto 0
     * Our unranking follows the same lexicographic order. */
    idx_t i0, i1, i2, rem = j;

    for (i0 = k - 1; i0 >= 2; --i0) {
        idx_t cnt = binom2(i0);         /* number of combinations with this i0 */
        if (rem < cnt) break;
        rem -= cnt;
    }
    for (i1 = i0 - 1; i1 >= 1; --i1) {
        idx_t cnt = i1;                 /* combinations with this i1 */
        if (rem < cnt) break;
        rem -= cnt;
    }
    /* Remaining rem determines i2 in descending order */
    i2 = i1 - 1 - rem;

    /* ---------- construct the value ----------
       start with the leading 1 bit; set all other bits except the
       positions i0, i1, i2. */
    seq_t val = (seq_t)1 << (L - 1);     /* leading bit */

    for (idx_t pos = 0; pos < k; ++pos) {
        if (pos != i0 && pos != i1 && pos != i2) {
            val |= ((seq_t)1 << pos);
        }
    }

    return val;
}
