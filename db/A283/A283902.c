/*
 * A283902  Relative of Hofstadter Q-sequence: a(-701) = 4, a(-700) = 702,
 *          a(-699) = 4, a(-698) = 702; thereafter a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)).
 * https://oeis.org/A283902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n >= -697 initial values:
 *            a(-701)=4, a(-700)=702, a(-699)=4, a(-698)=702, a(n)=0 for n <=
 *            -702. Offset: a(1) is the first term
 * Range:     u8 none, u16 n <= 1455, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define OFFSET 702
#define MAX_N 12000  /* safe upper bound (the sequence ends at 11969) */
static seq_t cache[MAX_N + OFFSET + 1];
static int computed_max = -698;          /* last index whose value is known */
static bool initialized = false;

static void ensure_initialized(void)
{
    if (initialized) return;
    /* set all to 0 */
    for (int i = 0; i < MAX_N + OFFSET + 1; ++i) cache[i] = 0;
    /* seed the given negative terms */
    cache[-701 + OFFSET] = 4;
    cache[-700 + OFFSET] = 702;
    cache[-699 + OFFSET] = 4;
    cache[-698 + OFFSET] = 702;
    /* indices <= -702 are already 0 */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* just in case */
    if (n > (idx_t)MAX_N) return 0;
    int target = (int)n;   /* n <= MAX_N fits in int */
    ensure_initialized();
    while (computed_max < target)
    {
        ++computed_max;
        int i = computed_max;
        seq_t prev1 = cache[i - 1 + OFFSET];
        seq_t prev2 = cache[i - 2 + OFFSET];
        seq_t v1, v2;

        /* Compute a(i - a(i-1)) */
        if (prev1 <= (seq_t)(i + 702))
        {
            int idx1 = i - (int)prev1;          /* idx1 >= -702 */
            v1 = cache[idx1 + OFFSET];
        }
        else
        {
            v1 = 0;
        }

        /* Compute a(i - a(i-2)) */
        if (prev2 <= (seq_t)(i + 702))
        {
            int idx2 = i - (int)prev2;          /* idx2 >= -702 */
            v2 = cache[idx2 + OFFSET];
        }
        else
        {
            v2 = 0;
        }

        cache[i + OFFSET] = v1 + v2;
    }
    return cache[target + OFFSET];
}
