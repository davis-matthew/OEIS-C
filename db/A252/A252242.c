/*
 * A252242  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 1 3 4 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 1 3 4 6 or 7.
 * https://oeis.org/A252242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 12, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252242 – compute a(n) via its empirical recurrence
   a(n) = 4104*a(n-3) – 32768*a(n-6)   (n > 7)
   Initial terms (offset 1):
   a(1) = 74824
   a(2) = 2568432
   a(3) = 65274264
   a(4) = 301990464
   a(5) = 10519316352
   a(6) = 267361717440
   a(7) = 1236950585856
   The function memoises results so that any order of calls is
   supported.  It uses a static array sized for up to n = 10000,
   which is sufficient for the range required by the tests. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Allocate enough space for n up to 10000 (1‑based indexing). */
    static seq_t values[10001 + 1];
    static bool initialized = false;
    static idx_t max_computed = 0;

    if (!initialized) {
        /* Set the first seven terms explicitly. */
        static const seq_t init_vals[8] = {
            0,
            (seq_t)74824ULL,
            (seq_t)2568432ULL,
            (seq_t)65274264ULL,
            (seq_t)301990464ULL,
            (seq_t)10519316352ULL,
            (seq_t)267361717440ULL,
            (seq_t)1236950585856ULL
        };
        for (idx_t i = 1; i <= 7; ++i) {
            values[i] = init_vals[i];
        }
        max_computed = 7;
        initialized = true;
    }

    if (n <= max_computed) {
        return values[n];
    }

    /* If n exceeds the pre‑allocated size, enlarge the buffer. */
    if (n > 10000) {
        /* Exceeds safe limit; compute on the fly without storing. */
        seq_t result = 0;
        seq_t prev3 = 0, prev6 = 0;
        /* Find the largest index we have already computed. */
        idx_t last = max_computed;
        /* Compute all intermediate values needed, keeping only
           the last six required values. */
        seq_t buf[6];
        for (idx_t i = 1; i <= last; ++i) {
            buf[i % 6] = values[i];
        }
        for (idx_t i = last + 1; i <= n; ++i) {
            buf[i % 6] = (seq_t)4104 * buf[(i - 3) % 6]
                        - (seq_t)32768 * buf[(i - 6) % 6];
        }
        return buf[n % 6];
    }

    /* Compute missing terms up to n, storing them. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        values[i] = (seq_t)4104 * values[i - 3]
                    - (seq_t)32768 * values[i - 6];
    }
    max_computed = n;
    return values[n];
}
