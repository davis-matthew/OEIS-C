/*
 * A386285  Values of u in the quartets (3, u, v, w) of type 2; i.e., values
 *          of u for solutions to 3(3 + u) = v(v - w), in positive integers,
 *          with v > 1, sorted by nondecreasing values of u; see Comments.
 * https://oeis.org/A386285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A386285.
 * A(n) is the nth smallest value of u appearing in all
 * quadruples (3,u,v,w) of type 2:
 *   3*(3+u) = v*(v-w)
 * where 3, u, v, w are distinct positive integers and 3 < v.
 * The values are sorted by nondecreasing u; if a given u occurs
 * in several distinct quadruples it is counted as many times.
 */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build. */

static seq_t *u_vals = NULL;   /* stored u values (with duplicates) */
static size_t u_len  = 0;      /* current number of stored values      */
static size_t u_cap  = 0;      /* allocated capacity                  */
static idx_t  next_u = 1;      /* next u to inspect                     */
static idx_t  max_need = 0;    /* largest request seen so far            */

/* Allocate or enlarge the storage array. */
static void grow_capacity(void)
{
    if (u_cap == 0) {
        u_cap = 64;
        u_vals = (seq_t *)malloc(u_cap * sizeof(seq_t));
    } else {
        u_cap *= 2;
        u_vals = (seq_t *)realloc(u_vals, u_cap * sizeof(seq_t));
    }
}

/* Ensure we have at least n values in the array. */
static void ensure_seq(idx_t n)
{
    if (n <= max_need) return;        /* already enough */

    while (u_len < n) {
        /* Current u */
        seq_t u = (seq_t)next_u;

        /* Skip u == 3: it would duplicate the fixed value m = 3. */
        if (u != 3) {
            seq_t K = (seq_t)3 * (u + (seq_t)3);

            /* Enumerate divisors d of K with d^2 <= K. */
            for (idx_t d = 1; (seq_t)d * (seq_t)d <= K; ++d) {
                if (K % (seq_t)d != 0) continue;

                /* Consider the divisor pair (d, K/d).  The
                 * viable quadruple comes from setting v = K/d
                 * and e = d (since then e < v and w = v-e > 0). */
                seq_t v = K / d;   /* v > d >= 1 */
                if (v <= 3) continue;           /* v must be > 3 (m) */

                seq_t w = v - (seq_t)d; /* w = v - e */
                if (w == 0) continue;      /* w must be positive */
                if (w == (seq_t)3) continue;/* distinct from m */
                if (w == u) continue;      /* distinct from u */
                if (u == v) continue;      /* distinct from v */

                /* All distinctness conditions satisfied; record u. */
                if (u_len == u_cap) grow_capacity();
                u_vals[u_len++] = u;
            }
        }

        ++next_u;  /* proceed to the next u for future calls */
    }
    max_need = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* index 0 is outside the OEIS offset */
    ensure_seq(n);
    return u_vals[(size_t)(n - 1)];
}
