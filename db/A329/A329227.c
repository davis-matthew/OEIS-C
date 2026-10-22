/*
 * A329227  Products of consecutive terms of the Padovan sequence A000931.
 * https://oeis.org/A329227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n-1) * P(n) where P is the Padovan sequence defined by
 *            P(0)=0, P(1)=0, P(2)=1 and P(n) = P(n-2) + P(n-3) (n >= 3)
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 45, u64 n <= 84, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises Padovan numbers up to the
   required index to keep each call O(n) worst‑case,
   but overall linear for successive calls. */

seq_t a(idx_t n)
{
    /* Static cache for Padovan values up to 10000,
       enough for all expected test indices (the
       problem statement limits n to ~10000). */
    static seq_t pad[10001];
    static idx_t pad_up_to = 2;   /* highest index computed */
    static int pad_inited = 0;    /* 0 = not initialized */

    /* First call: initialise base cases P(0)=0, P(1)=0, P(2)=1 */
    if (!pad_inited) {
        pad[0] = 0;
        pad[1] = 0;
        pad[2] = 1;
        pad_inited = 1;
    }

    /* If we haven't computed Padovan up to the requested n,
       extend the cache.  The array is statically sized to
       accommodate all indices up to 10000. */
    if (n > pad_up_to) {
        idx_t upper = n;
        if (upper >= 10001) {             /* guard against overflow */
            upper = 10000;
        }
        for (idx_t i = pad_up_to + 1; i <= upper; ++i) {
            pad[i] = pad[i - 2] + pad[i - 3];
        }
        pad_up_to = upper;
    }

    /* For indices below 2 the product is zero. */
    if (n < 2)
        return 0;

    /* a(n) = P(n-1) * P(n) */
    return pad[n - 1] * pad[n];
}
