/*
 * A353592  Inverse of the permutation A355506 (a(n)+1 is at distance >= a(n)
 *          from a(n)).
 * https://oeis.org/A353592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS seq_t a(idx_t n): inverse of permutation A355506
 *   a(0) = 0, a(1) = 1.
 *   For n >= 2:
 *     if n is even:  a[n] = smallest unused positive integer,
 *     if n is odd :  a[n] = smallest unused positive integer + n.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Internal static state ---------------------------------------------------- */
static seq_t *seq_cache = NULL;     /* cached terms, index 0..seq_len */
static size_t seq_cap   = 0;         /* allocated capacity of seq_cache   */
static idx_t seq_len    = 0;         /* highest index computed (>= 1)     */

static unsigned char *used     = NULL;  /* bit array marking used integers   */
static size_t used_cap        = 0;      /* allocated size of `used` array     */
static seq_t small_first_unused = 2;    /* smallest unused integer (≥ 2)     */

/* Lazily initialise static arrays ---------------------------------------- */
static void init_static(void)
{
    if (seq_cache != NULL)
        return;

    /* initialise terms cache: indices 0 and 1 */
    seq_cap = 4;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_cache[0] = 0;                 /* a(0) */
    seq_cache[1] = 1;                 /* a(1) */
    seq_len = 1;                       /* maximum computed index */

    /* initialise used array: mark 1 as used */
    used_cap = 8;
    used = (unsigned char *)malloc(used_cap * sizeof(unsigned char));
    memset(used, 0, used_cap);
    used[1] = 1;                       /* number 1 is already used */
}

/* Ensure `used` array can store at least index `v` ------------------------ */
static void ensure_used_cap(seq_t v)
{
    if ((size_t)v < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= (size_t)v)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!tmp)
        abort(); /* allocation failed */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used = tmp;
    used_cap = new_cap;
}

/* Ensure `seq_cache` can store at least index `n` ------------------------ */
static void ensure_seq_cap(idx_t n)
{
    if ((size_t)n < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* Main API --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_static();

    if (n == 0)
        return seq_cache[0];
    if (n <= seq_len)
        return seq_cache[n];

    /* extend sequence up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i)
    {
        /* find current smallest unused integer */
        while ((size_t)small_first_unused < used_cap && used[(size_t)small_first_unused])
            ++small_first_unused;

        seq_t value;
        if ((i & 1) == 0)   /* even index */
        {
            value = small_first_unused;
            ensure_used_cap(value);
            used[(size_t)value] = 1;
            ++small_first_unused; /* next step will skip this number */
        }
        else                /* odd index (i>=3) */
        {
            value = small_first_unused + (seq_t)i;
            ensure_used_cap(value);
            used[(size_t)value] = 1;
            /* small_first_unused stays unchanged */
        }

        ensure_seq_cap(i);
        seq_cache[i] = value;
    }

    seq_len = n;
    return seq_cache[n];
}
