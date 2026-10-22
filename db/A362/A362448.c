/*
 * A362448  Triangle T(n,k) (n >= 0, 0 <= k <= n) read by rows: T(n,k) = 1 if
 *          the English names for n and k have a letter in common, otherwise
 *          0.
 * https://oeis.org/A362448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, word
 * Formula:   a(n) = 1 if the English names of the row index r and the column
 *            index k (derived from n by triangular numbering) share a letter,
 *            otherwise 0
 * Range:     u8 n <= 8821, u16 n <= 8821, u32 n <= 8821, u64 n <= 8821,
 *            u128 n <= 8821, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* ---------- types supplied by the build ---------- */
/* idx_t : unsigned 64‑bit integer
   seq_t : unsigned integer of any width */

/* ---------- bit masks for letters ---------- */
typedef uint32_t mask_t;       /* 32 bits are enough for 26 letters */

/* ---------- string fragments of English number names ---------- */
static const char *ONES_STR[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};

static const char *TENS_STR[10] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

static const char *SCALE_STR[6] = {
    "thousand","million","billion","trillion","quadrillion","quintillion"
};

/* ---------- bit masks for the fragments ---------- */
static mask_t MASK_ONES[20];
static mask_t MASK_TENS[10];
static mask_t MASK_SCALE[6];
static mask_t MASK_HUNDRED;

/* ---------- helper: build mask from a literal string ---------- */
static mask_t mask_from_literal(const char *s) {
    mask_t m = 0;
    for (; *s; ++s) {
        char c = *s;
        if (c >= 'a' && c <= 'z')
            m |= (mask_t)1 << (c - 'a');
    }
    return m;
}

/* ---------- lazy initialization of the masks ---------- */
static bool MASKS_INIT = false;
static void init_masks(void) {
    if (MASKS_INIT) return;
    for (int i = 0; i < 20; ++i) MASK_ONES[i] = mask_from_literal(ONES_STR[i]);
    for (int i = 0; i < 10; ++i) MASK_TENS[i] = mask_from_literal(TENS_STR[i]);
    for (int i = 0; i < 6; ++i)  MASK_SCALE[i] = mask_from_literal(SCALE_STR[i]);
    MASK_HUNDRED = mask_from_literal("hundred");
    MASKS_INIT = true;
}

/* ---------- recursively compute the mask for a number ---------- */
static mask_t english_mask(idx_t n) {
    if (n == 0) return MASK_ONES[0];
    if (n < 20) return MASK_ONES[n];
    if (n < 100) {
        mask_t m = MASK_TENS[n / 10];
        if (n % 10) m |= MASK_ONES[n % 10];
        return m;
    }
    if (n < 1000) {
        mask_t m = english_mask(n / 100);
        m |= MASK_HUNDRED;
        if (n % 100) m |= english_mask(n % 100);
        return m;
    }
    /* n >= 1000: find highest power of 1000 */
    idx_t pow = 1000;
    int idx = 0;                /* index into MASK_SCALE */
    while ((__uint128_t)pow * 1000 <= n) {
        pow *= 1000;
        ++idx;
    }
    idx_t high = n / pow;
    idx_t low  = n % pow;
    mask_t m = english_mask(high);
    m |= MASK_SCALE[idx];
    if (low) m |= english_mask(low);
    return m;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* Initialise masks once */
    init_masks();

    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    unsigned __int128 tr = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tr;

    mask_t mr = english_mask(r);
    mask_t mk = english_mask(k);

    return (seq_t)((mr & mk) ? 1 : 0);
}
