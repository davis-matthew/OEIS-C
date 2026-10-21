/*
 * A260580  Table read by rows: n-th row contains numbers not occurring
 *          earlier, that can be written as (p+q)/2 where p is the n-th odd
 *          prime, q <= p.
 * https://oeis.org/A260580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 245, u16 n <= 7895, u32 n <= 7895, u64 n <= 7895,
 *            u128 n <= 7895, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence computed by generating a table row by row.
    The n-th odd prime p is used, and for all primes q <= p
    we form m = (p+q)/2.  Every m that has not appeared in any
    previous rows is appended to the sequence.  The sequence is
    the rows concatenated in order (row 1, row 2, …).  The algorithm
    maintains a dynamic list of used values with a bitmap for values
    up to SIZE_MAX and a vector for larger values.  Primes are
    generated incrementally using the provided is_prime(idx_t) utility.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t x) */

/* --- prime cache ---------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* prime_list[0] = 2, prime_list[1] = 3, … */
static size_t   prime_count = 0;      /* number of primes stored */
static size_t   prime_cap   = 0;

static void add_prime(uint64_t p)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_count == prime_cap) {
        prime_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_count++] = p;
}

/* ensure that prime_list contains primes[0…need] (inclusive) */
static void ensure_prime_count(size_t need)
{
    if (prime_count > need)
        return;

    if (prime_count == 0) {
        add_prime(2);             /* seed with 2 */
    }
    /* next candidate should be the smallest odd number above the last prime */
    uint64_t cand = (prime_list[prime_count - 1] == 2) ? 3U
                                                         : prime_list[prime_count - 1] + 2U;
    while (prime_count <= need) {
        if (is_prime(cand))
            add_prime(cand);
        cand += 2U;      /* only odd candidates */
    }
}

/* --- sequence cache ------------------------------------------------- */
static seq_t *seq_values = NULL;  /* seq_values[0] = a(1) */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static void append_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 16;
        seq_values = (seq_t *)realloc(seq_values, seq_cap * sizeof(seq_t));
    }
    seq_values[seq_len++] = v;
}

/* --- memory for membership test ------------------------------------- */
static uint8_t *used_bool = NULL;      /* bitmap for values <= SIZE_MAX */
static size_t  used_bool_len = 0;      /* current allocated size */

static seq_t *used_vec   = NULL;      /* vector for values > SIZE_MAX */
static size_t used_vec_len = 0;
static size_t used_vec_cap = 0;

static const seq_t MAX_AS_SEQ = (seq_t)SIZE_MAX;

static void mark_used(seq_t v)
{
    if (v <= MAX_AS_SEQ) {
        size_t idx = (size_t)v;
        if (idx >= used_bool_len) {
            size_t newlen = used_bool_len ? used_bool_len : 1024;
            while (idx >= newlen)
                newlen <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(used_bool, newlen);
            if (tmp) {
                memset(tmp + used_bool_len, 0, newlen - used_bool_len);
                used_bool = tmp;
            }
            used_bool_len = newlen;
        }
        used_bool[idx] = 1;
    } else {
        /* linear search; values are rare */
        for (size_t i = 0; i < used_vec_len; ++i)
            if (used_vec[i] == v)
                return;
        if (used_vec_len == used_vec_cap) {
            used_vec_cap = used_vec_cap ? used_vec_cap << 1 : 16;
            used_vec = (seq_t *)realloc(used_vec, used_vec_cap * sizeof(seq_t));
        }
        used_vec[used_vec_len++] = v;
    }
}

static bool is_used(seq_t v)
{
    if (v <= MAX_AS_SEQ) {
        size_t idx = (size_t)v;
        return idx < used_bool_len && used_bool[idx];
    }
    for (size_t i = 0; i < used_vec_len; ++i)
        if (used_vec[i] == v)
            return true;
    return false;
}

/* --- the sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1, this case is unused */

    size_t target = (size_t)n;
    if (seq_len >= target)
        return seq_values[target - 1];

    /* process rows until we have at least target terms */
    size_t row_idx = 1;   /* row 1 uses odd prime prime_list[1] = 3 */
    while (seq_len < target) {
        ensure_prime_count(row_idx);

        seq_t p = (seq_t)prime_list[row_idx];    /* n-th odd prime */
        /* generate values from q=prime_list[1]..prime_list[row_idx] */
        for (size_t j = 1; j <= row_idx; ++j) {
            seq_t q = (seq_t)prime_list[j];
            seq_t cand = (p + q) >> 1;   /* same as (p+q)/2, guaranteed even sum */
            if (!is_used(cand)) {
                append_seq(cand);
                mark_used(cand);
                if (seq_len >= target)
                    break;
            }
        }
        ++row_idx;
    }
    return seq_values[target - 1];
}
