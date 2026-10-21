/*
 * A272267  The union of centered square numbers (A001844) and centered
 *          9-gonal numbers (A060544).
 * https://oeis.org/A272267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = merge of centered square numbers (2*k*(k-1)+1) and
 *            centered 9‑gonal numbers (9*k*(k-1)/2+1), with duplicates
 *            removed
 * Range:     u8 n <= 17, u16 n <= 301, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i = 1, j = 1;            /* indices for the two sequences   */
    seq_t last = 0;                /* last value added to the union   */
    idx_t count = 0;               /* number of terms generated       */

    while (count < n) {
        /* centered square: 2*k*(k-1)+1   */
        seq_t v1 = (seq_t)2 * (seq_t)i * (i - 1) + 1;
        /* centered 9‑gonal: 9*k*(k-1)/2+1   */
        seq_t v2 = (seq_t)9 * (seq_t)j * (j - 1) / 2 + 1;
        seq_t next;

        if (v1 < v2) {
            next = v1;
            i++;                       /* move to next centered square   */
        } else if (v2 < v1) {
            next = v2;
            j++;                       /* move to next centered 9‑gonal   */
        } else {                      /* same value – already in set    */
            next = v1;
            i++; j++;                  /* skip both indices              */
        }

        if (next > last) {            /* avoid duplicate entries         */
            last = next;
            count++;
        }
    }
    return last;
}
