/*
 * A382093  Sequence where k is appended after every (k-1)! occurrences of 1,
 *          with multiple values following a 1 listed in order.
 * https://oeis.org/A382093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1001, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A382093 – append k after every (k‑1)! occurrences of 1.
 *  The sequence is produced by emitting a 1, then checking
 *  all k ≥ 2.  When the count of 1’s since the last k equals
 *  (k‑1)!, we output k and reset that counter.
 *  This algorithm reproduces the pattern
 *  1,2,1,2,3,1,2,1,2,3,1,2,1,2,3,4,…   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0 but safe */

    enum { MAX_K = 20 };           /* sufficient for n ≤ 10 000 */
    seq_t threshold[MAX_K + 1];
    seq_t counter[MAX_K + 1];
    int i;

    /* initialise arrays */
    for (i = 0; i <= MAX_K; ++i) {
        threshold[i] = 0;
        counter[i]  = 0;
    }

    /* determine highest k needed: (k‑1)! ≤ n */
    int max_k = 0;
    seq_t fact = 1;
    seq_t n_seq = (seq_t)n;
    for (i = 2; i <= MAX_K; ++i) {
        fact = fact * (i - 1);          /* (i‑1)! */
        if (fact > n_seq) break;
        threshold[i] = fact;
        max_k = i;
    }

    idx_t generated = 0;
    seq_t value = 0;

    while (generated < n) {
        /* emit a 1 */
        value = 1;
        ++generated;
        if (generated == n) return value;

        /* after each 1, increment k‑counters and output any due ks */
        for (i = 2; i <= max_k; ++i) {
            ++counter[i];
            if (counter[i] == threshold[i]) {
                value = (seq_t)i;
                counter[i] = 0;
                ++generated;
                if (generated == n) return value;
            }
        }
    }

    return value;   /* unreachable, keeps compiler happy */
}
