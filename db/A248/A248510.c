/*
 * A248510  Decimal expansion of von Klitzing constant R(K) (ohms).
 * https://oeis.org/A248510
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of von Klitzing constant R(K),
   offset 5 (a(5) = 2, a(6) = 5, a(7) = 8, …). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Denominator of the constant */
static const seq_t R_K_DEN = (seq_t)213914163877964163ULL;

/* Cached data for memoization */
static bool init_done      = false;
static seq_t *cache        = NULL;
static idx_t cache_cap     = 0;
static idx_t last_computed = 0;          /* highest cached position (0‑based) */
static seq_t rem_frac      = 0;          /* remainder after integer part */
static uint8_t int_digits[5];

/* Initialise cache with the integer part and remaining remainder */
static void a_init(void) {
    const char *num_str = "5521725125000000000000";
    seq_t rem = 0;
    seq_t quot = 0;
    for (const char *p = num_str; *p; ++p) {
        int d = *p - '0';
        seq_t val = rem * (seq_t)10 + (seq_t)d;
        seq_t qdig = val / R_K_DEN;
        rem = val % R_K_DEN;
        quot = quot * (seq_t)10 + qdig;
    }
    /* Extract integer digits (25812). */
    seq_t tmp = quot;
    for (int i = 4; i >= 0; --i) {
        int_digits[i] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    /* Allocate cache for the first five digits. */
    cache_cap = 32;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    for (idx_t i = 0; i < 5; ++i) cache[i] = (seq_t)int_digits[i];
    last_computed = 4;          /* positions 5..9 correspond to indices 0..4 */
    rem_frac = rem;
    init_done = true;
}

/* Ensure cache can hold 'needed' elements. */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;                /* undefined for n < offset */

    if (!init_done) a_init();

    idx_t pos = n - 5;                          /* position in cache */

    if (pos > last_computed) {
        ensure_capacity(pos + 1);
        for (idx_t i = last_computed + 1; i <= pos; ++i) {
            seq_t digit;
            if (i <= 4) {
                digit = (seq_t)int_digits[i];
            } else {
                seq_t mult = rem_frac * (seq_t)10;
                digit = mult / R_K_DEN;
                rem_frac = mult % R_K_DEN;
            }
            cache[i] = digit;
        }
        last_computed = pos;
    }
    return cache[pos];
}
