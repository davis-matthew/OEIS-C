/*
 * A358967  a(n+1) gives the number of occurrences of the smallest digit of
 *          a(n) so far, up to and including a(n), with a(0)=0.
 * https://oeis.org/A358967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 687, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358967:  a(0)=0; a(n+1) = number of occurrences of the
   smallest decimal digit of a(n) in all decimal digits of a(0..n). */

#include <stddef.h>
#include <stdbool.h>

/* smallest decimal digit in x (x>=0) */
static unsigned smallest_digit(seq_t x)
{
    if (x == (seq_t)0)
        return 0;
    unsigned min_d = 10;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        if (d < min_d)
            min_d = d;
        x /= (seq_t)10;
    }
    return min_d;
}

/* add the digits of x to the cumulative frequency array freq[10] */
static void add_digits(seq_t x, unsigned *freq)
{
    if (x == (seq_t)0) {
        freq[0]++;              /* digit 0 occurs once in "0" */
        return;
    }
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        freq[d]++;
        x /= (seq_t)10;
    }
}

static seq_t *cache_seq = NULL;     /* sequence values   */
static unsigned *cache_freq = NULL; /* cumulative digit counts for each computed term   */
static size_t cache_len = 0;        /* number of terms currently stored */
static size_t cache_cap = 0;        /* allocated capacity of cache_seq   */
static unsigned global_freq[10] = {0}; /* cumulative counts of digits so far */
static bool initialized = false;

/* ensure cache can hold at least 'need' terms, computing missing terms */
static void ensure_cache(idx_t need)
{
    if (!initialized) {               /* first call: initialize sequence */
        cache_cap = 16;
        cache_seq = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache_seq) return;       /* allocation failure – should not happen */
        /* a(0)=0 */
        cache_seq[0] = (seq_t)0;
        cache_len = 1;
        /* init cumulative frequencies with digits of a(0) */
        for (int i = 0; i < 10; ++i) global_freq[i] = 0;
        add_digits(cache_seq[0], global_freq);
        initialized = true;
    }

    while (cache_len <= (size_t)need) {   /* compute terms up to 'need' */
        /* compute a[cache_len] based on a[cache_len-1] */
        unsigned sd = smallest_digit(cache_seq[cache_len - 1]);
        seq_t next_val = global_freq[sd];

        /* ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_buf = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
            if (!new_buf) return;       /* allocation failure */
            cache_seq = new_buf;
            cache_cap = new_cap;
        }

        cache_seq[cache_len] = next_val;
        cache_len++;

        /* update cumulative frequencies with digits of the new term */
        add_digits(next_val, global_freq);
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache_seq[(size_t)n];
}
