/*
 * A378637  Largest m <= n such that phi(m) divides n, where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A378637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Computes a(n) = largest m ≤ n such that φ(m) divides n.
 *   Here φ(m) is Euler's totient function.  The implementation
 *   pre‑computes φ values up to the largest argument seen and
 *   searches downward from n until the divisibility condition holds.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"          /* provides seq_t phi(idx_t m); */

static seq_t *phi_cache = NULL;   /* indices 0..cache_cap are stored, unused 0 */
static idx_t  cache_cap  = 0;     /* current maximum index cached */

/* Ensure φ(m) is known for all 1 ≤ m ≤ up.  Expand the cache if needed. */
static void
ensure_phi_upto(idx_t up)
{
    if (up <= cache_cap)
        return;

    size_t new_cap = up + 1;
    seq_t *new_cache = (seq_t *)realloc(phi_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                 /* memory allocation failure */

    for (idx_t i = cache_cap + 1; i <= up; ++i)
        new_cache[i] = phi((idx_t)i);

    phi_cache = new_cache;
    cache_cap  = up;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)               /* undefined for n < 1 */
        return 0;

    ensure_phi_upto(n);

    for (idx_t m = n; m > 0; --m) {   /* search downward */
        seq_t phi_m = phi_cache[m];
        if ((idx_t)n % (idx_t)phi_m == 0)
            return (seq_t)m;
    }
    return 1;               /* fallback (should never reach here) */
}
