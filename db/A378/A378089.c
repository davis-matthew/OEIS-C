/*
 * A378089  Irregular triangle read by rows in which row n lists the numbers k
 *          such that phi(k)/tau(k) = n.
 * https://oeis.org/A378089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378089: irregular triangle read by rows.
   The n-th term is the n-th element read row‑by‑row where
   row n lists the numbers k with φ(k)/τ(k) = n.
   This implementation enumerates integers k, computes φ(k) and τ(k),
   determines the ratio, and stores each k in the appropriate row.
   The rows are accumulated in static arrays so that calls can
   arrive in any order.  The code is valid C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <new>
#endif

/* ---------- Static state ---------- */
static uint32_t *spf            = NULL;   /* smallest prime factor */
static size_t   spf_limit       = 0;      /* current limit of spf array */

static seq_t  **row_data        = NULL;   /* arrays holding each row */
static size_t  *row_len         = NULL;   /* current length of each row   */
static size_t  *row_cap         = NULL;   /* current capacity of each row*/
static size_t   row_max         = 0;       /* largest row index allocated */

static size_t   total_terms     = 0;       /* total terms gathered so far */
static size_t   largest_k       = 0;       /* largest k processed          */

/* ---------- Helper: extend spf array ---------- */
static void
extend_spf(size_t new_limit)
{
    if (new_limit <= spf_limit)
        return;

    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_spf)
        exit(EXIT_FAILURE);

    for (size_t i = 2; i <= new_limit; ++i)
    {
        if (new_spf[i] == 0)          /* i is prime */
        {
            new_spf[i] = (uint32_t)i;
            if (i <= new_limit / i)
            {
                for (size_t j = i + i; j <= new_limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ---------- Helper: allocate or enlarge a row ---------- */
static void
ensure_row(size_t r)
{
    if (r <= row_max)
        return;

    size_t new_sz = r + 1;
    seq_t **new_data = (seq_t **)realloc(row_data, new_sz * sizeof(seq_t *));
    size_t *new_len  = (size_t *)realloc(row_len,  new_sz * sizeof(size_t));
    size_t *new_cap  = (size_t *)realloc(row_cap,  new_sz * sizeof(size_t));

    if (!new_data || !new_len || !new_cap)
        exit(EXIT_FAILURE);

    row_data = new_data;
    row_len  = new_len;
    row_cap  = new_cap;

    for (size_t i = row_max + 1; i < new_sz; ++i)
    {
        row_data[i] = NULL;
        row_len[i]  = 0;
        row_cap[i]  = 0;
    }
    row_max = r;
}

/* ---------- Helper: add a value to a row ---------- */
static void
add_term_to_row(size_t r, seq_t k)
{
    if (r == 0)
        return;                          /* φ(k)/τ(k) can never be 0 */

    ensure_row(r);

    /* Allocate the row if it does not exist yet. */
    if (row_data[r] == NULL)
    {
        row_cap[r] = 4;
        row_data[r] = (seq_t *)malloc(row_cap[r] * sizeof(seq_t));
        if (!row_data[r])
            exit(EXIT_FAILURE);
        /* No need to initialise elements here – they are constructed on use. */
    }
    else if (row_len[r] == row_cap[r])
    {
        size_t old_cap = row_cap[r];
        row_cap[r] <<= 1;                 /* double capacity */
        seq_t *tmp = (seq_t *)realloc(row_data[r], row_cap[r] * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        row_data[r] = tmp;

        /* Construct newly allocated elements only in C++ mode. */
#ifdef __cplusplus
        for (size_t i = old_cap; i < row_cap[r]; ++i)
            new (&row_data[r][i]) seq_t();
#endif
    }

    /* Construct the new element if it was not yet constructed. */
    size_t idx = row_len[r];
#ifdef __cplusplus
    new (&row_data[r][idx]) seq_t(k);
#else
    row_data[r][idx] = k;
#endif
    ++row_len[r];
    ++total_terms;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* below offset, undefined */

    size_t target = (size_t)n;

    /* Generate terms until we have at least 'target' of them. */
    while (total_terms < target)
    {
        size_t new_limit = (spf_limit == 0) ? 1000 : spf_limit * 2;
        extend_spf(new_limit);

        /* Process k from largest_k+1 up to new_limit. */
        for (size_t k = largest_k + 1; k <= new_limit; ++k)
        {
            seq_t phi = (seq_t)k;
            size_t tau = 1;
            size_t m = k;

            while (m > 1)
            {
                uint32_t p = spf[m];
                size_t exp = 0;
                while (m % p == 0)
                {
                    m /= p;
                    ++exp;
                }
                /* φ(k) = ∏ p^(e−1)(p−1) */
                phi = phi / (seq_t)p * ((seq_t)p - 1);
                tau *= (exp + 1);
            }

            if (phi % tau == 0)
            {
                seq_t r = phi / tau;
                if (r >= 1)
                    add_term_to_row((size_t)r, (seq_t)k);
            }
        }

        largest_k = new_limit;
    }

    /* Locate the 'target'-th term among rows. */
    size_t cumulative = 0;
    for (size_t r = 1; r <= row_max; ++r)
    {
        if (row_len[r] == 0)
            continue;
        if (cumulative + row_len[r] >= target)
            return row_data[r][target - cumulative - 1];

        cumulative += row_len[r];
    }

    return 0;         /* should never reach here */
}
