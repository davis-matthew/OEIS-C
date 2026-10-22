/*
 * A301574  a(n) = distance from n to nearest 3-smooth number (A003586).
 * https://oeis.org/A301574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = distance from n to the nearest 3‑smooth number (A003586)
 *            Computed by brute‑force enumeration of all numbers of the form
 *            2^i·3^j that are ≤ n + best. best starts at n (the maximal
 *            possible distance) and is tightened whenever a closer 3‑smooth
 *            number is found. The algorithm runs in O((log n)...
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1, n should be ≥1 */

    seq_t best  = (seq_t)n;              /* initial upper bound */
    seq_t limit = (seq_t)n + best;       /* current search limit n + best */

    seq_t pow2 = (seq_t)1;

    /* iterate over powers of 2 up to the current limit */
    while (pow2 <= limit) {
        seq_t pow3 = (seq_t)1;
        /* iterate over powers of 3 for each power of 2 */
        while (1) {
            seq_t prod;
            /* compute product safely: use 128‑bit intermediate for 64‑bit types */
            if (sizeof(seq_t) <= 8) { /* 8, 16, 32 or 64‑bit */
                unsigned __int128 temp = (unsigned __int128)pow2 *
                                          (unsigned __int128)pow3;
                if (temp > (unsigned __int128)limit) break;
                prod = (seq_t)temp;
            } else { /* arbitrary‑precision or 128‑bit types */
                prod = pow2 * pow3;
                if (prod > limit) break;
            }

            /* update best distance */
            seq_t diff = (prod > n) ? (prod - n) : (n - prod);
            if (diff < best) {
                best  = diff;
                limit = (seq_t)n + best;  /* tighten search window */
            }

            /* advance to next power of 3, stop if would exceed limit */
            if (pow3 > limit / 3) break;
            pow3 *= 3;
        }

        /* advance to next power of 2, stop if would exceed limit */
        if (pow2 > limit / 2) break;
        pow2 <<= 1;
    }

    return best;
}
