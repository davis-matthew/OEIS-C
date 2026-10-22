/*
 * A344634  Numbers k such that half the numbers from 0 to k inclusive contain
 *          the digit "0".
 * https://oeis.org/A344634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest k (k>0) such that among the numbers 0…k, exactly
 *            half contain the digit “0”. The sequence terms are generated
 *            incrementally by scanning forward and maintaining the difference
 *            between the counts of “0‑containing” and “non‑zero” numbers
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* computed terms */
static size_t cache_cap = 0, cache_size = 0;

static uint64_t cur = 1;             /* current k being examined */
static uint8_t digits[20];           /* decimal digits of cur (least significant first) */
static int len = 1;                  /* number of active digits */
static int zero_count = 0;           /* how many zero digits are present in cur */
static int64_t diff = 0;             /* (k+1) - 2*count_no_zero(k)   */
static bool initialized = false;    /* one‑time initialization flag */

static void init(void)
{
    cache_cap = 32;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    memset(digits, 0, sizeof(digits));
    digits[0] = 1;
    cur = 1;
    zero_count = 0;                 /* 1 has no zero */
    diff = 0;                       /* k=1 satisfies the condition */
    cache[0] = cur;
    cache_size = 1;
    initialized = true;
}

static void increment(void)
{
    /* Increment cur and update digits/zero_count.
       This is a classic increment by 1 algorithm on decimal digits.   */
    int pos = 0;
    int carry = 1;
    while (carry) {
        int old = digits[pos];
        int newdigit = old + carry;
        if (newdigit == 10) {
            newdigit = 0;
            carry = 1;
        } else {
            carry = 0;
        }
        if (old == 0) zero_count--;          /* a zero disappeared   */
        if (newdigit == 0) zero_count++;     /* a zero appeared      */
        digits[pos] = (uint8_t)newdigit;
        pos++;
        if (pos == len && carry) {           /* create a new most significant digit  */
            digits[pos] = 1;
            len++;
            carry = 0;
        }
    }
    cur++;                                  /* cur now holds the new number */
}

static void grow_cache(void)
{
    if (cache_size >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;                  /* undefined, but return 0 */

    while (cache_size < n) {
        /* find the next k > cur where diff becomes 0            */
        while (true) {
            increment();                   /* cur := cur+1, update digits/zero_count   */
            diff += (zero_count > 0) ? 1 : -1;
            if (diff == 0) break;
        }
        grow_cache();
        cache[cache_size] = cur;           /* store new term */
        cache_size++;
    }
    return cache[n - 1];
}
