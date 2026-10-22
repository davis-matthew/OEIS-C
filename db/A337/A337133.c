/*
 * A337133  Numbers of the form x^2+2*y^3+3*z^4 for nonnegative x, y, z.
 * https://oeis.org/A337133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form x^2 + 2*y^3 + 3*z^4 for nonnegative integers x, y, z. */
/* a(1) is the smallest such number (0), a(2)=1, a(3)=2, … */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----- static tables (built once) ----- */
    static seq_t *seq = NULL;    /* sorted unique values */
    static size_t seq_len = 0;   /* number of values collected */

    /* ----- build sequence if not yet done ----- */
    if (seq == NULL) {
        /* numeric bound to generate values up to */
        const idx_t LIMIT = 3000000;   /* should yield >10 000 terms */

        /* bit array to mark which numbers are representable */
        uint8_t *seen = (uint8_t*)calloc((size_t)LIMIT + 1, 1);
        if (seen == NULL) return 0; /* allocation failure – unlikely */

        /* enumerate all combinations of non‑negative x, y, z */
        for (idx_t z = 0; ; ++z) {
            seq_t z4 = (seq_t)z * (seq_t)z * (seq_t)z * (seq_t)z;   /* z^4   */
            seq_t base_z = 3 * z4;
            if (base_z > LIMIT) break;                    /* further z only increases */

            for (idx_t y = 0; ; ++y) {
                seq_t y3 = (seq_t)y * (seq_t)y * (seq_t)y;   /* y^3   */
                seq_t base_zy = base_z + 2 * y3;
                if (base_zy > LIMIT) break;                /* further y only increases */

                /* inner loop over x, stopping when the value exceeds LIMIT */
                for (idx_t x = 0; ; ++x) {
                    seq_t val = base_zy + (seq_t)x * (seq_t)x;
                    if (val > LIMIT) break;
                    seen[(size_t)val] = 1;
                }
            }
        }

        /* count number of seen values */
        seq_len = 0;
        for (idx_t i = 0; i <= LIMIT; ++i)
            if (seen[(size_t)i]) ++seq_len;

        /* allocate array to store the sorted values */
        seq = (seq_t*)malloc(seq_len * sizeof(seq_t));
        if (seq == NULL) { free(seen); return 0; }

        /* collect the values in order */
        idx_t pos = 0;
        for (idx_t i = 0; i <= LIMIT && pos < seq_len; ++i)
            if (seen[(size_t)i]) seq[pos++] = (seq_t)i;

        free(seen);
    }

    /* ---- answer the query ---- */
    if (n == 0) return 0;                 /* undefined, return 0 */
    if ((idx_t)n > seq_len) return 0;     /* requested term out of range */
    return seq[(size_t)(n - 1)];
}
