/*
 * A287443  Start with 0 and repeatedly substitute 0->012, 1->201, 2->021.
 * https://oeis.org/A287443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A287443 (fixed point of the morphism
   0 -> 012, 1 -> 201, 2 -> 021).  For a given 1‑based index n,
   the function interprets n-1 in base‑3 and walks down the substitution
   tree, starting from symbol 0.  The resulting symbol (0, 1 or 2) is
   returned as a seq_t value. */
static const int trans[3][3] = {
    {0, 1, 2},   // 0 -> 012
    {2, 0, 1},   // 1 -> 201
    {0, 2, 1}    // 2 -> 021
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          // safety for n < 1
    idx_t m = n - 1ULL;                  // zero‑based position
    unsigned int digits[64];             // enough for any 64‑bit number
    int dcnt = 0;

    /* Extract base‑3 digits from least significant to most */
    while (m > 0) {
        digits[dcnt++] = (unsigned int)(m % 3ULL);
        m /= 3ULL;
    }

    /* Position 0 corresponds to the initial symbol 0 */
    if (dcnt == 0) return (seq_t)0;

    int s = 0;                            // start symbol
    /* Process digits from most significant to least */
    for (int i = dcnt - 1; i >= 0; --i) {
        unsigned int d = digits[i];
        s = trans[s][d];
    }

    return (seq_t)s;
}
