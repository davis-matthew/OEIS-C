/*
 * A265859  Unluckiness of n.
 * https://oeis.org/A265859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1-A145649(n))*A260438(n) // (unlucky numbers get the
 *            step at which they are removed, lucky numbers get 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for previously computed unluckiness values */
static idx_t cache_max = 0;          /* maximum n that has been computed */
static seq_t *unluckiness = NULL;   /* array indexed by n, value = removal step (0 if lucky) */

/* recompute unluckiness for all n <= max_n */
static void recompute_unluckiness(idx_t max_n)
{
    /* free previous cache */
    if (unluckiness != NULL)
        free(unluckiness);

    /* allocate new cache and set to 0 */
    unluckiness = (seq_t *)calloc((size_t)max_n + 1, sizeof(seq_t));
    if (unluckiness == NULL)
        return;  /* allocation failed; caller will return 0 */

    /* quick handling of very small n */
    if (max_n < 1)
        return;

    /* two buffers used alternately to simulate the sieve */
    idx_t *cur  = (idx_t *)malloc((size_t)max_n * sizeof(idx_t));
    idx_t *nxt  = (idx_t *)malloc((size_t)max_n * sizeof(idx_t));
    if (cur == NULL || nxt == NULL)
    {
        free(unluckiness); free(cur); free(nxt);
        unluckiness = NULL;
        return;
    }

    /* initial list: 1..max_n */
    for (idx_t i = 0; i < max_n; ++i)
        cur[i] = i + 1;

    idx_t curSize = max_n;
    idx_t step = 1;       /* step numbering starts at 1 for evens */
    idx_t index = 1;      /* index of current step size (0‑based) */

    while (index < curSize)
    {
        idx_t stepSize = cur[index];      /* value used as step length */

        /* elimination: walk through current list, keep survivors */
        idx_t newSize = 0;
        for (idx_t pos = 0; pos < curSize; ++pos)
        {
            if (((pos + 1) % stepSize) == 0)
            {
                /* element is removed at this step */
                unluckiness[cur[pos]] = (seq_t)step;
            }
            else
            {
                nxt[newSize++] = cur[pos];
            }
        }

        /* prepare for next round */
        idx_t *tmp = cur;   /* swap cur and nxt pointers */
        cur = nxt;
        nxt = tmp;
        curSize = newSize;

        ++step;  /* increment removal step count */

        /* decide whether the step-size element persisted */
        int removed = ((index + 1) % stepSize) == 0;
        if (!removed)
            ++index;       /* move to next remaining element */
        /* if removed, keep index same (it now points to the next element after the former one) */

        /* loop stops automatically if index >= curSize */
    }

    /* clean up temporary buffers */
    free(cur);
    free(nxt);

    /* update cache */
    cache_max = max_n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, return 0 */

    if (n > cache_max)
        recompute_unluckiness(n);

    if (unluckiness == NULL)
        return (seq_t)0;   /* should not happen, but safety */

    return unluckiness[n];
}
