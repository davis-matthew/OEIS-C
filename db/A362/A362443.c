/*
 * A362443  Numbers k with property that the set of letters in the English
 *          name for k does not contain two letters that are adjacent in the
 *          alphabet.
 * https://oeis.org/A362443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th non‑negative integer whose English name
 *  (in lower case, no hyphens or spaces) contains no pair of
 *  letters that are adjacent in the alphabet.
 *  Example: 3 ("three") is valid, 5 ("five") is not (e&f).
 */

/*  Build on demand a static table of the (finite) valid numbers,
 *  then answer the requested n‑th index (indices start at 1).
 *  The sequence contains only numbers 0..99; any larger number
 *  is guaranteed to introduce a consecutive pair of letters
 *  (see OEIS comment).
 */

#include <stdbool.h>
#include <stddef.h>  /* size_t */

/* English words for 0..19 */
static const char *const ONES[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};

/* English words for tens 20,30,...,90 */
static const char *const TENS[] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

/* Test whether number k (0 <= k <= 99) satisfies the property. */
static inline bool check_number(idx_t k)
{
    unsigned mask = 0;            /* 26 bits for letters a..z   */

    if (k < 20) {
        const char *w = ONES[k];
        for (; *w; ++w) mask |= 1u << (*w - 'a');
    } else {
        unsigned tens   = k / 10;
        unsigned units  = k % 10;
        const char *tw = TENS[tens];
        for (; *tw; ++tw) mask |= 1u << (*tw - 'a');
        if (units) {
            const char *uw = ONES[units];
            for (; *uw; ++uw) mask |= 1u << (*uw - 'a');
        }
    }

    /* Check for consecutive alphabet letters */
    for (unsigned i = 0; i < 25; ++i)
        if ((mask & (1u << i)) && (mask & (1u << (i + 1))))
            return false;
    return true;
}

/* Generate the whole table into a static array (max 100 entries). */
static void generate_terms(seq_t *table, size_t *len)
{
    size_t cnt = 0;
    for (idx_t n = 0; n <= 99; ++n) {      /* only 0..99 are relevant */
        if (check_number(n)) {
            table[cnt++] = (seq_t)n;
        }
    }
    *len = cnt;
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t  table[100];     /* arbitrary‑precision or integral type */
    static size_t table_len = 0;

    if (table_len == 0)            /* first call: fill the cache */
        generate_terms(table, &table_len);

    if (n >= 1 && n <= (idx_t)table_len)
        return table[n-1];          /* indices in OEIS start at 1 */
    return (seq_t)0;                /* undefined indices return 0 */
}
