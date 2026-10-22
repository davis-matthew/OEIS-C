/*
 * A392837  a(n) = 2^(n-1)*(L(4*n + 1) + 9 - 5*n)/5, where L(n) = A000032(n).
 * https://oeis.org/A392837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(L(4*n + 1) + 9 - 5*n)/5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_capacity, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where L(0)=2, L(1)=1, and L(k)=L(k-1)+L(k-2) */

/* Include the utilities required for big integer operations */
#include "../utils/ensure_capacity.h"
#include "../utils/pow2.h"

/* Static cache for Lucas numbers up to the maximum requested index */
static seq_t *lucas_cache = NULL;   /* array of Lucas numbers */
static size_t lucas_len   = 0;       /* number of valid entries in lucas_cache */
static size_t lucas_cap   = 0;       /* allocated capacity of lucas_cache */

/* Ensure lucas_cache contains L(0) … L(up) */
static void
ensure_lucas(idx_t up)
{
    if (up < lucas_len) return;                /* already available */

    /* enlarge the buffer if necessary */
    size_t needed = (size_t)up + 1;
    ensure_capacity(needed, &lucas_cap, (void **)&lucas_cache, sizeof(seq_t));

    /* initialise the first two Lucas numbers if this is the first call */
    if (lucas_len == 0) {
        lucas_cache[0] = (seq_t)2;
        if (needed > 1) {
            lucas_cache[1] = (seq_t)1;
            lucas_len = 2;
        } else {
            lucas_len = 1;
        }
    }

    /* compute missing values */
    while (lucas_len <= up) {
        lucas_cache[lucas_len] =
            lucas_cache[lucas_len - 1] + lucas_cache[lucas_len - 2];
        ++lucas_len;
    }
}

/* Sequence a(n) for OEIS A392837 */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;              /* special case a(0)=1 */

    /* compute the required Lucas number L(4*n + 1) */
    idx_t k = 4 * n + 1;
    ensure_lucas(k);
    seq_t L = lucas_cache[k];

    /* compute the numerator: (L + 9 - 5*n) * 2^(n-1) */
    seq_t factor = pow2(n - 1);                      /* 2^(n-1) */
    seq_t inner   = L + (seq_t)9 - (seq_t)5 * n;     /* L + 9 - 5*n */
    seq_t num     = inner * factor;                  /* numerator */

    /* divide by 5; the division is exact by definition of the sequence */
    return num / (seq_t)5;
}
