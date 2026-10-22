/*
 * A378196  Number of 2-colorings of length n without an arithmetic
 *          progression of length 4.
 * https://oeis.org/A378196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n that contain no
 *            monochromatic arithmetic progression of length 4. The sequence
 *            is 0 for n ≥ 35 because the van der Waerden number W(2,4)=35
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine is valid for any integer type seq_t and idx_t supplied by the build
   environment.  Avoids external type redeclarations. */
#include <stdbool.h>
#include <stdint.h>

#define MAX_LEN 34                       /* longest non‑zero term */

static seq_t counts[MAX_LEN + 1];        /* counts for lengths 0 … 34   */
static bool  initialized = false;        /* one‑time initialization */
static uint8_t bits[MAX_LEN + 1];        /* bits of the current prefix */

/* Recursive DFS counting all valid prefixes. */
static void dfs(idx_t pos)
{
    /* Register current prefix length. */
    counts[pos]++;

    if (pos == (idx_t)MAX_LEN) return;   /* maximal length reached */

    /* Try both colours for the next position. */
    for (uint8_t color = 0; color <= 1; ++color) {
        bits[pos] = color;

        /* Check whether adding this colour creates a monochromatic 4‑AP
           ending at position 'pos'. */
        bool ok = true;
        for (idx_t d = 1; d * 3 <= pos; ++d) {
            idx_t i0 = pos - 3 * d;
            idx_t i1 = pos - 2 * d;
            idx_t i2 = pos - d;
            if (bits[i0] == bits[i1] &&
                bits[i0] == bits[i2] &&
                bits[i0] == bits[pos]) {
                ok = false;
                break;
            }
        }
        if (ok) dfs(pos + 1);
    }
}

/* Public entry function. */
seq_t a(idx_t n)
{
    /* Compute all counts once. */
    if (!initialized) {
        dfs(0);
        initialized = true;
    }

    if (n > (idx_t)MAX_LEN) return 0;   /* beyond van der Waerden threshold */
    return counts[n];
}
