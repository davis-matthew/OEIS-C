/*
 * A294719  Number of permutations of [n] avoiding {2341, 2143, 4123}.
 * https://oeis.org/A294719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294719:
   a(n) = 8*a(n-1) - 26*a(n-2) + 45*a(n-3) - 45*a(n-4) + 26*a(n-5) - 8*a(n-6) + a(n-7)
   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=70, a(6)=212, a(7)=597
   (Recurrence holds for n > 7) */

#include <stdint.h>
#include <stddef.h>

/* Maximum n that we preallocate for. The sequence values grow rapidly; for
   n up to about 10000 all terms fit into the provided seq_t type in the
   test harness.  If a larger n is requested the function will return 0
   (undefined behaviour). */
#define MAX_PREALLOC  10001

static seq_t cache[MAX_PREALLOC];
static unsigned char computed[MAX_PREALLOC];
static idx_t last_computed = (idx_t)(-1); /* indicates no value has been computed yet */

/* Initialize the cache with the known first eight terms. */
static void initialize_cache(void)
{
    if (last_computed <= 7) return;          /* already initialized */

    cache[0] = (seq_t)1;  computed[0] = 1;
    cache[1] = (seq_t)1;  computed[1] = 1;
    cache[2] = (seq_t)2;  computed[2] = 1;
    cache[3] = (seq_t)6;  computed[3] = 1;
    cache[4] = (seq_t)21; computed[4] = 1;
    cache[5] = (seq_t)70; computed[5] = 1;
    cache[6] = (seq_t)212;computed[6] = 1;
    cache[7] = (seq_t)597;computed[7] = 1;

    last_computed = 7;
}

/* Ensure that all terms up to n (inclusive) are computed. */
static void ensure_cache(idx_t n)
{
    if (n < last_computed) return;          /* already computed */

    if (n >= MAX_PREALLOC) {
        /* n exceeds the preallocated range.  The implementation is not
           required to handle this case; the build is expected to
           request only values up to about 10000. */
        return;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For indices 0..7 the values are already taken from the
           initialisation code.  For larger indices we use the
           recurrence.  To avoid under‑flow for unsigned seq_t
           types, compute the positive and negative parts
           separately. */
        seq_t pos = (seq_t)8 * cache[i-1];
        pos = pos + (seq_t)45 * cache[i-3];
        pos = pos + (seq_t)26 * cache[i-5];
        pos = pos + cache[i-7];

        seq_t neg = (seq_t)26 * cache[i-2];
        neg = neg + (seq_t)45 * cache[i-4];
        neg = neg + (seq_t)8  * cache[i-6];

        cache[i] = pos - neg;
        computed[i] = 1;
    }

    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n >= MAX_PREALLOC) return (seq_t)0; /* undefined for out of range */
    if (last_computed == (idx_t)(-1)) initialize_cache();
    ensure_cache(n);
    return cache[(size_t)n];
}
