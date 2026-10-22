/*
 * A343271  From left to right: the k-th binary digit of a(n) is the most
 *          frequent digit among the first k binary digits of n (in case of a
 *          tie, take the k-th binary digit of n).
 * https://oeis.org/A343271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the binary string obtained by, for each k from 1 to
 *            len(n), * setting the k‑th bit of a(n) to the most frequent bit
 *            among the first k bits * of n (ties resolved by taking the k‑th
 *            bit of n)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any n >= 0 (works for arbitrary seq_t sizes and idx_t = uint64_t). */
seq_t a(idx_t n) {
    /* Determine bit length of n (1 for n==0). */
    int len;
    if (n == 0) {
        len = 1;
    } else {
        idx_t tmp = n;
        len = 0;
        while (tmp) {
            len++;
            tmp >>= 1;
        }
    }

    seq_t result = 0;      /* The computed a(n). */
    int ones = 0;          /* Number of 1's seen so far. */

    /* Process bits from most significant to least significant. */
    for (int pos = 0; pos < len; pos++) {
        /* Extract the current bit of n. */
        int bit = (int)((n >> (len - 1 - pos)) & 1ull);
        ones += bit;
        int zeros = (pos + 1) - ones;

        int outbit;
        if (ones > zeros)
            outbit = 1;
        else if (zeros > ones)
            outbit = 0;
        else
            outbit = bit;   /* Tie, use the current bit. */

        result = (result << 1) | (seq_t)outbit;
    }

    return result;
}
