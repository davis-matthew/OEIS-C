/*
 * A247360  Numbers with French name ending in a consonant.
 * https://oeis.org/A247360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = nth positive integer whose French name ends with a
 *            consonant * (i.e., the last letter of the French word is a
 *            consonant). * The function determines this by testing each
 *            number sequentially * and caching the results. The test for a
 *            single number uses the * French naming rules up to the thou...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const bool consonant_end[100] = {
    /*  0  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
     * 20 21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39
     * 40 41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59
     * 60 61  62  63  64  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79
     * 80 81  82  83  84  85  86  87  88  89  90  91  92  93  94  95  96  97  98  99
     */
    /* 0 */  false,
    /* 1 */  true, true, true, false, true, true, true, true, true,  /* 1-9 */
    /*10 */  true, false, false, false, false, false, false, true, true, true, /*10-19*/
    /*20 */  true, true, true, true, false, true, true, true, true, true,  /*20-29*/
    /*30 */  false, true, true, true, false, true, true, true, true, true, /*30-39*/
    /*40 */  false, true, true, true, false, true, true, true, true, true, /*40-49*/
    /*50 */  false, true, true, true, false, true, true, true, true, true, /*50-59*/
    /*60 */  false, true, true, true, false, true, true, true, true, true, /*60-69*/
    /*70 */  true, false, false, false, false, false, false, true, true, true, /*70-79*/
    /*80 */  true, true, true, true, false, true, true, true, true, true, /*80-89*/
    /*90 */  true, false, false, false, false, false, false, true, true, true  /*90-99*/
};

static bool ends_in_consonant(idx_t n)
{
    if (n == 0) return false;          /* 0 → zéro (ends with o) */

    if (n < 100) {
        return consonant_end[(int)n];
    }

    if (n < 1000) {
        idx_t r = n % 100;
        if (r == 0) return true;       /* multiples of 100 end with "cent" */
        return consonant_end[(int)r];
    }

    idx_t rest = n % 1000;
    if (rest == 0) return false;       /* numbers like 1000, 2000, ... end with "mille" */
    return ends_in_consonant(rest);     /* recurse on the lower 3‑digit group */
}

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of elements stored */
static idx_t last_checked = 0; /* largest number examined so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0 but safe return */

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_checked = 0;
    }

    /* Keep generating until we have at least n terms */
    while (cache_len < n) {
        ++last_checked;                      /* next candidate number */

        if (ends_in_consonant(last_checked)) {
            if (cache_len == cache_cap) {     /* grow the cache */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
