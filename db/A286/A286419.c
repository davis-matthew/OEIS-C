/*
 * A286419  {0100->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9672 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of the 0100→1 transformed Thue‑Morse word (OEIS A286419).
   The algorithm scans the Thue‑Morse sequence, detects every disjoint
   occurrence of the pattern 0100 and replaces it by a single 1, otherwise
   keeping the bits unchanged.  Since the pattern does not overlap,
   the process is linear in the number of output bits.  The Thue‑Morse
   bit at position k (0‑based) is the parity of the popcount of k, which
   is obtained by successive XOR‑shifts. */
static inline uint8_t thue_t(idx_t k)
{
    /* parity of popcount(k) using only bitwise operations */
    uint64_t x = k;
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (uint8_t)(x & 1u);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 */
    idx_t outcnt = 0;                       /* number of output bits produced */
    uint8_t buf[4];
    int blen = 0;                            /* current buffer length < 4 */
    seq_t outbit = 0;                        /* most recent output bit */

    for (idx_t k = 0; ; ++k) {
        /* generate next Thue‑Morse bit */
        uint8_t tb = thue_t(k);
        buf[blen++] = tb;

        if (blen == 4) {
            /* check for the pattern 0100 */
            if (buf[0] == 0 && buf[1] == 1 && buf[2] == 0 && buf[3] == 0) {
                outbit = (seq_t)1;
                ++outcnt;
                if (outcnt == n) return outbit;
                blen = 0;                    /* pattern consumed */
                continue;
            }
            /* emit the first bit of the buffer */
            outbit = (seq_t)buf[0];
            ++outcnt;
            if (outcnt == n) return outbit;
            /* shift the remaining three bits left */
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            blen = 3;
        }
    }
}
