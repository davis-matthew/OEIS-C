/*
 * A335087  Row sums of A335436.
 * https://oeis.org/A335087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2) + 16*a(n-3) - 4*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Init: a(0)=1, a(1)=7, a(2)=34, a(3)=150 */

#include <stddef.h>
#include <stdint.h>

static seq_t cache[10001];
static idx_t computed_until = (idx_t)-1;  /* indicates uninitialized */

/* Compute the sequence up to index n (inclusive) if necessary. */
static void ensure_computed(idx_t n)
{
    if (computed_until == (idx_t)-1) {
        /* First time: set base values. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)34;
        cache[3] = (seq_t)150;
        computed_until = 3;
    }
    if (n <= computed_until)
        return;                         /* already computed */

    /* Extend if n is larger than our static array. */
    if (n >= 10001) {
        /* In this build we assume n never exceeds 10000. */
        return;
    }

    for (idx_t i = computed_until + 1; i <= n; ++i) {
        /* recurrence: a(i) = 8*a(i-1) -20*a(i-2) +16*a(i-3) -4*a(i-4) */
        seq_t t1 = (seq_t)8   * cache[i - 1];
        seq_t t2 = (seq_t)20  * cache[i - 2];
        seq_t t3 = (seq_t)16  * cache[i - 3];
        seq_t t4 = (seq_t)4   * cache[i - 4];
        cache[i] = t1 - t2 + t3 - t4;
    }
    computed_until = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)34;
    if (n == 3) return (seq_t)150;

    ensure_computed(n);
    return cache[n];
}
