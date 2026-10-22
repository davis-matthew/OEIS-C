/*
 * A321464  Reverse nonzero digits in ternary expansion of n and convert back
 *          to decimal.
 * https://oeis.org/A321464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 244, u16 n <= 9266, u32 n <= 9266, u64 n <= 9266,
 *            u128 n <= 9266, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Convert n to base‑3, reverse the order of its non‑zero digits
   while keeping zeros in their original places, then convert back to
   decimal. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Temporary storage of ternary digits (least significant first). */
    uint8_t lsb[64];
    unsigned len = 0;
    uint64_t t = n;
    while (t > 0) {
        lsb[len++] = t % 3;
        t /= 3;
    }

    /* Convert to most significant first order. */
    uint8_t msb[64];
    for (unsigned i = 0; i < len; ++i) {
        msb[i] = lsb[len - 1 - i];
    }

    /* Collect positions and values of non‑zero digits. */
    unsigned nz_pos[64];
    uint8_t  nz_val[64];
    unsigned nz_cnt = 0;
    for (unsigned i = 0; i < len; ++i) {
        if (msb[i] != 0) {
            nz_pos[nz_cnt] = i;
            nz_val[nz_cnt] = msb[i];
            ++nz_cnt;
        }
    }

    /* Place reversed non‑zero digits back into their positions. */
    for (unsigned i = 0; i < nz_cnt; ++i) {
        msb[nz_pos[i]] = nz_val[nz_cnt - 1 - i];
    }

    /* Reconstruct the decimal value. */
    seq_t res = 0;
    for (unsigned i = 0; i < len; ++i) {
        res = res * (seq_t)3 + msb[i];
    }
    return res;
}
