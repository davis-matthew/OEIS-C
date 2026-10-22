/*
 * A362218  Three-column array read by rows: row n gives the unique ordered
 *          primitive Pythagorean triple (a,b,c) with a<b such that (b+c)/a =
 *          n.
 * https://oeis.org/A362218
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the entry at position n of the concatenated * 3‑column
 *            array of primitive Pythagorean triples (a,b,c) with * a<b and
 *            (b+c)/a = k, for k=3,4,5,… * The triple for odd k is
 *            (k,(k^2–1)/2,(k^2+1)/2). * The triple for even k is
 *            (2k,k^2–1,k^2+1). * We map n≥3 to the appropriate triple and
 *            component
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* undefined for n<3, return 0 */

    /* zero based offset in the concatenated sequence */
    idx_t offset = n - 3;
    idx_t r = 3 + offset / 3;       /* ratio k */
    idx_t pos = offset % 3;         /* 0 → a, 1 → b, 2 → c */

    seq_t R = (seq_t)r;             /* promote to seq_t for arithmetic */

    if (r & 1) {                    /* odd k */
        switch (pos) {
            case 0: return R;               /* a = k */
            case 1: return (R * R - 1) / 2; /* b = (k^2-1)/2 */
            case 2: return (R * R + 1) / 2; /* c = (k^2+1)/2 */
            default: return 0;
        }
    } else {                        /* even k */
        seq_t a_val = 2 * R;          /* a = 2k */
        seq_t r2 = R * R;             /* k^2 */
        switch (pos) {
            case 0: return a_val;         /* a = 2k */
            case 1: return r2 - 1;       /* b = k^2-1 */
            case 2: return r2 + 1;       /* c = k^2+1 */
            default: return 0;
        }
    }
}
