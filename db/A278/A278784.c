/*
 * A278784  Numbers m such that A000041(m) is of the form 2^7 * k for odd k.
 * https://oeis.org/A278784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = m such that p(m) ≡ 128 (mod 256),
   where p(m) is the partition number with recurrence
   p(0)=1,
   p(n)=∑_{k=1..∞} (-1)^{k-1}[p(n-k(3k-1)/2)+p(n-k(3k+1)/2)]   (mod 256) */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *p_cache = NULL;        /* partition numbers modulo 256 */
static size_t p_cap   = 0;            /* capacity of p_cache */
static size_t last_m  = 0;            /* last m computed */

static seq_t *found     = NULL;      /* list of m satisfying condition */
static size_t found_cap = 0;          /* capacity of found array */
static size_t found_len = 0;          /* number of terms already found */

static bool initialized = false;

/* Ensure p_cache has room for at least size entries */
static void ensure_p_capacity(size_t size)
{
    if (size < p_cap)
        return;
    size_t new_cap = (p_cap == 0) ? 128 : p_cap;
    while (new_cap <= size)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    p_cache = tmp;
    p_cap   = new_cap;
}

/* Ensure found array has room for at least size entries */
static void ensure_found_capacity(size_t size)
{
    if (size < found_cap)
        return;
    size_t new_cap = (found_cap == 0) ? 256 : found_cap;
    while (new_cap <= size)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    found = tmp;
    found_cap = new_cap;
}

/* Initialise data structures on first call */
static void init(void)
{
    if (initialized)
        return;
    ensure_p_capacity(1);
    p_cache[0] = (seq_t)1;   /* p(0)=1 */
    last_m = 0;
    ensure_found_capacity(1);
    found_len = 0;
    initialized = true;
}

/* Compute partition numbers up to new_m (inclusive) and collect
   all m whose p(m) % 256 == 128. */
static void compute_up_to(size_t new_m)
{
    for (size_t m = last_m + 1; m <= new_m; ++m) {
        ensure_p_capacity(m);
        int acc = 0;
        for (int k = 1; ; ++k) {
            size_t pent1 = (size_t)k * (3 * k - 1) / 2;
            if (pent1 > m)
                break;
            int sign = (k & 1) ? 1 : -1;
            acc += sign * (int)p_cache[m - pent1];
            size_t pent2 = (size_t)k * (3 * k + 1) / 2;
            if (pent2 <= m)
                acc += sign * (int)p_cache[m - pent2];
        }
        acc %= 256;
        if (acc < 0)
            acc += 256;
        p_cache[m] = (seq_t)acc;

        if (acc == 128) {
            ensure_found_capacity(found_len + 1);
            found[found_len++] = (seq_t)m;
        }
    }
    last_m = new_m;
}

/* Ensure that found_len >= n, computing more values if necessary */
static void ensure_n_terms(idx_t n)
{
    init();
    while ((idx_t)found_len < n) {
        /* increase search range by a factor, e.g. 50% more */
        size_t next_m = last_m + ((last_m >> 1) > 1000 ? (last_m >> 1) : 1000);
        compute_up_to(next_m);
    }
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, but guard */
    ensure_n_terms(n);
    return found[n - 1];
}
