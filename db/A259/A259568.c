/*
 * A259568  Numbers following gaps in the sequence of base-4 numbers that
 *          don't contain 0.
 * https://oeis.org/A259568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of all base‑4 numbers that contain no digit 0 and whose
   least significant digit is 1.  The nth such number (offset 1) is returned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: sequence starts at n=1 */

    idx_t len = 1;          /* current number of base‑4 digits          */
    idx_t count = 1;        /* how many numbers have this length       */
    idx_t cumulative = 1;   /* total numbers up to this length         */

    /* find the length len of the desired term */
    while (cumulative < n) {
        ++len;
        count *= 3;          /* each new digit (except the last) has 3 choices */
        cumulative += count;
    }

    /* offset of the term within the block of this length  (0‑based) */
    idx_t prev_cum = cumulative - count;
    idx_t offset   = n - prev_cum - 1;

    /* extract the (len‑1) base‑3 digits of offset, most significant first */
    idx_t digits[64] = {0};
    idx_t tmp = offset;
    for (idx_t i = len - 1; i > 0; --i) {
        digits[i - 1] = tmp % 3;
        tmp /= 3;
    }

    /* build the base‑4 number: digits+1, followed by the final 1 (%)*/
    seq_t val = 0;
    for (idx_t i = 0; i < len - 1; ++i) {
        val = val * 4 + (seq_t)(digits[i] + 1);
    }
    val = val * 4 + 1;        /* the fixed least significant digit 1 */

    return val;
}
