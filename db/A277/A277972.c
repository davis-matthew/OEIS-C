/*
 * A277972  a(1) = 1. a(n) is the smallest unlisted number, the name of which
 *          contains the first letter of the name of a(n-1) in English.
 * https://oeis.org/A277972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) is the smallest unlisted number whose English name contains
 *            the first letter of the name of a(n-1). This implementation
 *            builds the sequence on demand, caching used numbers and letter
 *            masks
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   word_mask_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/word_mask_string.h"

/* ------------------------------------------------------------------ */
/* Name masks for units, tens, and magnitude words                    */
/* ------------------------------------------------------------------ */
static const char *const unit_words[20] = {
    "zero","one","two","three","four","five","six","seven",
    "eight","nine","ten","eleven","twelve","thirteen",
    "fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"
};

static const char *const tens_words[10] = {
    "","", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

static uint32_t unit_mask[20];
static uint32_t tens_mask[10];
static uint32_t mask_hundred;
static uint32_t mask_thousand;
static uint32_t mask_million;
static uint32_t mask_billion;

/* ------------------------------------------------------------------ */
/* State for used numbers and their letter masks                       */
/* ------------------------------------------------------------------ */
static bool *used = NULL;          /* used[i] == true iff number i is in sequence */
static uint32_t *mask_of = NULL;   /* cached letter mask of number i */
static idx_t used_cap = 0;         /* current capacity of used/mask_of */

/* Ensure arrays cover indices up to 'limit' (inclusive). */
static void ensure_capacity(idx_t limit)
{
    if (limit < used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= limit) new_cap <<= 1;

    bool *new_used  = (bool *)realloc(used, new_cap * sizeof(bool));
    uint32_t *new_mask = (uint32_t *)realloc(mask_of, new_cap * sizeof(uint32_t));
    for (idx_t i = used_cap; i < new_cap; ++i) {
        new_used[i] = false;
        new_mask[i] = 0;
    }
    used = new_used;
    mask_of = new_mask;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute the letter mask of a number (recursively).                  */
/* ------------------------------------------------------------------ */
static uint32_t number_mask(idx_t n)
{
    uint32_t m = 0;
    if (n >= 1000000000ULL) {           /* billions */
        idx_t billions = n / 1000000000ULL;
        m |= number_mask(billions);
        m |= mask_billion;
        n %= 1000000000ULL;
    }
    if (n >= 1000000ULL) {               /* millions */
        idx_t millions = n / 1000000ULL;
        m |= number_mask(millions);
        m |= mask_million;
        n %= 1000000ULL;
    }
    if (n >= 1000ULL) {                  /* thousands */
        idx_t thousands = n / 1000ULL;
        m |= number_mask(thousands);
        m |= mask_thousand;
        n %= 1000ULL;
    }
    if (n >= 100ULL) {                   /* hundreds */
        idx_t hundreds = n / 100ULL;
        m |= unit_mask[hundreds];
        m |= mask_hundred;
        n %= 100ULL;
    }
    if (n >= 20ULL) {                    /* tens */
        idx_t tens = n / 10ULL;
        m |= tens_mask[tens];
        n %= 10ULL;
    }
    if (n > 0ULL) {                      /* unit < 20 */
        m |= unit_mask[n];
    }
    return m;
}

/* ------------------------------------------------------------------ */
/* First letter of the English name of a number                       */
/* ------------------------------------------------------------------ */
static const char first_letter_units[20] = {
    'z','o','t','t','f','f','s','s','e','n',
    't','e','t','t','f','f','s','s','e','n'
};
static const char first_letter_tens[10] = {
    ' ', ' ', 't','t','f','f','s','s','e','n'
};

static char first_letter_unit(idx_t x)            { return first_letter_units[(int)x]; }
static char first_letter_tens_digit(idx_t d)     { return first_letter_tens[(int)d]; }

static char first_letter_number(idx_t n)
{
    if (n == 0) return 'z';
    if (n >= 1000000000ULL)   return first_letter_number(n / 1000000000ULL);
    if (n >= 1000000ULL)      return first_letter_number(n / 1000000ULL);
    if (n >= 1000ULL)         return first_letter_number(n / 1000ULL);
    if (n >= 100ULL)          return first_letter_unit(n / 100ULL);
    if (n >= 20ULL)           return first_letter_tens_digit(n / 10ULL);
    return first_letter_unit(n);            /* n < 20 */
}

/* ------------------------------------------------------------------ */
/* Sequence storage                                                   */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;       /* array holding the sequence values */
static idx_t seq_len = 0;       /* number of terms currently computed */
static idx_t seq_cap = 0;       /* allocated capacity of seq */

/* Append a value to the sequence array, growing it if necessary. */
static void seq_append(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Lazy initialization of masks, constants and first element.        */
/* ------------------------------------------------------------------ */
static void seq_init(void)
{
    if (seq != NULL) return;   /* already initialized */

    /* Build masks for unit and tens words. */
    for (int i = 1; i < 20; ++i) unit_mask[i] = word_mask_string(unit_words[i]);
    for (int i = 2; i < 10; ++i) tens_mask[i] = word_mask_string(tens_words[i]);

    /* Masks for magnitude words. */
    mask_hundred   = word_mask_string("hundred");
    mask_thousand  = word_mask_string("thousand");
    mask_million   = word_mask_string("million");
    mask_billion   = word_mask_string("billion");

    /* Allocate initial space for used/mask_of. */
    ensure_capacity(16);

    /* Allocate space for the sequence itself. */
    seq = (seq_t *)malloc(16 * sizeof(seq_t));
    seq_cap = 16;
    seq_len = 0;

    /* First term is 1. */
    seq_append((seq_t)1);
    used[1] = true;
}

/* ------------------------------------------------------------------ */
/* Compute the sequence up to index n (1‑based).                      */
/* ------------------------------------------------------------------ */
static void compute_to_idx(idx_t n)
{
    seq_init();

    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        char letter = first_letter_number((idx_t)prev);
        int li = letter - 'a';          /* index 0..25 */

        /* Pointer for next candidate with this letter. */
        static idx_t ptr_letter[26] = {0};

        /* First time global init: set all to 1. */
        if (!ptr_letter[0]) {
            for (int k = 0; k < 26; ++k) ptr_letter[k] = 1;
        }

        idx_t candidate = ptr_letter[li];
        while (1) {
            ensure_capacity(candidate);
            if (!used[candidate]) {
                uint32_t mask = mask_of[candidate];
                if (mask == 0) mask_of[candidate] = mask = number_mask(candidate);
                if (mask & (1u << li)) break;
            }
            ++candidate;
        }

        /* Record the chosen number. */
        used[candidate] = true;
        seq_append((seq_t)candidate);
        ptr_letter[li] = candidate + 1;   /* Next search will start after this */
    }
}

/* ------------------------------------------------------------------ */
/* Public interface: return a(n) for any n >= 1.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_to_idx(n);
    return seq[n - 1];
}
