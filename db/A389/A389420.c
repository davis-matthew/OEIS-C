/*
 * A389420  Arrangement of numbers such that every digit rotation of the
 *          number n is included in ascending order after those of n-1.
 * https://oeis.org/A389420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 9722, u32 n <= 9722, u64 n <= 9722,
 *            u128 n <= 9722, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A389420.
   The sequence is obtained by, for each integer m ≥ 0,
   listing all cyclic rotations of its decimal digits (ignoring
   leading zeros), sorting them ascending, and concatenating those
   blocks in increasing order of m.  This code finds the block
   containing the requested index and returns the appropriate
   rotation. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* first term */

    /* find the block number m such that the cumulative length
       up to m exceeds n.  sum stores the total length processed. */
    idx_t sum = 1;                  /* block for 0 contributes one element */
    idx_t m = 0;
    while (1) {
        ++m;
        idx_t tmp = m;
        int d = 0;
        do { d++; tmp /= 10; } while (tmp > 0);   /* digit count of m */
        sum += d;
        if (sum > n) break;
    }

    /* recompute digit count of m for later use */
    idx_t tmp = m;
    int d = 0;
    do { d++; tmp /= 10; } while (tmp > 0);
    idx_t block_start = sum - d;     /* starting index of this block */
    idx_t pos = n - block_start;     /* position within the block */

    /* collect digits of m (most significant first) */
    int digits[20];
    tmp = m;
    for (int i = d - 1; i >= 0; --i) {
        digits[i] = tmp % 10;
        tmp /= 10;
    }

    /* generate all rotations, ignoring leading zeros */
    seq_t rot[20];
    for (int k = 0; k < d; ++k) {
        unsigned __int128 u = 0;
        int started = 0;
        for (int t = 0; t < d; ++t) {
            int dig = digits[(k + t) % d];
            if (!started) {
                if (dig == 0) continue;
                started = 1;
            }
            u = u * 10 + dig;
        }
        if (!started) u = 0;
        rot[k] = (seq_t)u;
    }

    /* insertion sort (d ≤ 20) */
    for (int i = 1; i < d; ++i) {
        seq_t v = rot[i];
        int j = i - 1;
        while (j >= 0 && rot[j] > v) {
            rot[j + 1] = rot[j];
            --j;
        }
        rot[j + 1] = v;
    }

    return rot[pos];
}
