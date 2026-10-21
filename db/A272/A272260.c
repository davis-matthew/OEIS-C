/*
 * A272260  Numbers that cause an infinite loop in Conway's PRIMEGAME.
 * https://oeis.org/A272260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: A272260, numbers that cause an infinite loop in Conway's
 * PRIMEGAME.  The function performs a brute‑force search; for each integer
 * it simulates the PRIMEGAME machine and uses Floyd's cycle‑finding to
 * detect whether the machine ever revisits a value.  The results are
 * cached to allow random access of an arbitrary term. */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t NUMS[14] = {
    (seq_t)17, (seq_t)78, (seq_t)19, (seq_t)23, (seq_t)29,
    (seq_t)77, (seq_t)95, (seq_t)77, (seq_t)1,  (seq_t)11,
    (seq_t)13, (seq_t)15, (seq_t)1,  (seq_t)55
};
static const seq_t DENOS[14] = {
    (seq_t)91, (seq_t)85, (seq_t)51, (seq_t)38, (seq_t)33,
    (seq_t)29, (seq_t)23, (seq_t)19, (seq_t)17, (seq_t)13,
    (seq_t)11, (seq_t)2,  (seq_t)7,  (seq_t)1
};

/* Compute the next integer produced from current value v. */
static seq_t next_val(seq_t v)
{
    for (int i = 0; i < 14; ++i) {
        seq_t num = NUMS[i];
        seq_t den = DENOS[i];
        /* use a 128‑bit intermediate to avoid overflow on u64 builds. */
        unsigned __int128 prod = (unsigned __int128)v * (unsigned __int128)num;
        if ((prod % (unsigned __int128)den) == 0)
            return (seq_t)(prod / (unsigned __int128)den);
    }
    /* The fraction 55/1 guarantees a hit, so this line is never reached. */
    return 0;
}

/* Detect whether the machine starting from 'start' eventually loops.
 * Uses Floyd's cycle‑finding algorithm with a generous step limit. */
static bool causesInfiniteLoop(idx_t start)
{
    const idx_t STEP_LIMIT = 2000;   /* enough for all known cycles */
    seq_t tortoise = next_val(start);
    seq_t hare = next_val(next_val(start));

    for (idx_t steps = 0; steps < STEP_LIMIT; ++steps) {
        if (tortoise == hare)
            return true;
        tortoise = next_val(tortoise);
        hare = next_val(next_val(hare));
    }
    return false;
}

/* Cached terms of the sequence to support random access and memoisation. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache has space for at least 'min_cap' elements. */
static void ensure_cache_cap(size_t min_cap)
{
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < min_cap)
            new_cap = min_cap;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined – offset starts at 1 */

    /* Generate terms until we have at least 'n' elements. */
    static idx_t tested = 0;
    while (cache_len < (size_t)n) {
        ++tested;
        if (causesInfiniteLoop(tested)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = tested;
        }
    }
    return cache[n - 1];
}
