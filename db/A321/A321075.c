/*
 * A321075  Digits of one of the two 11-adic integers sqrt(3).
 * https://oeis.org/A321075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = digit at position n in the 11‑adic expansion of one
 * square root of 3.  Computed by incremental Hensel lifting in base 11.
 *
 * The algorithm keeps the current digits of the 11‑adic integer X and the
 * corresponding convolution array representing X².  Each new digit a(k)
 * is chosen so that (X + a(k)·11^k)² ≡ 3 (mod 11^(k+1)).  Because
 * 2·X ≡ 1 (mod 11) for all k, the chosen digit satisfies
 *     a(k) ≡ (11 – coeff_k_of_X²) mod 11
 * where coeff_k_of_X² is the k‑th coefficient of X² after normalisation.
 *
 * The routine uses only integer arithmetic, works for any seq_t that fits
 * the required operations, and memoises previously computed digits so
 * that out‑of‑order calls are handled efficiently.
 */

#include <stdint.h>
#include <stdlib.h>

/* static state – shared between calls */
static uint8_t *digits = NULL;      /* a[0..]  */
static uint32_t *prod   = NULL;     /* convolutions of digits, normalised */
static size_t digits_cap = 0;
static size_t prod_cap   = 0;
static idx_t  computed   = 0;        /* number of digits already known */

/* enlarge the digits array to at least 'need' elements */
static void ensure_digits(size_t need)
{
    if (digits_cap < need) {
        size_t new_cap = digits_cap ? digits_cap : 4;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        /* initialise the new part with zeros */
        for (size_t i = digits_cap; i < new_cap; ++i) tmp[i] = 0;
        digits = tmp;
        digits_cap = new_cap;
    }
}

/* enlarge the prod array to at least 'need' elements */
static void ensure_prod(size_t need)
{
    if (prod_cap < need) {
        size_t new_cap = prod_cap ? prod_cap : 8;
        while (new_cap < need) new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(prod, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        /* initialise the new part with zeros */
        for (size_t i = prod_cap; i < new_cap; ++i) tmp[i] = 0;
        prod = tmp;
        prod_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* first time – initialise the state with a(0)=6 */
    if (computed == 0) {
        digits_cap = 4;
        digits = (uint8_t *)calloc(digits_cap, sizeof(uint8_t));
        prod_cap   = 8;
        prod       = (uint32_t *)calloc(prod_cap, sizeof(uint32_t));
        if (!digits || !prod) abort();

        digits[0] = 6;
        /* initialise prod[0] = 6*6 = 36 and normalise */
        prod[0] = 36;
        uint32_t carry = prod[0] / 11;
        prod[0] %= 11;
        prod[1] += carry;   /* prod[1] was 0, stays < 11 after this step */
        computed = 1;       /* a(0) known */
    }

    /* bring precomputation up to n */
    while (computed <= n) {
        idx_t k = computed;            /* next digit index to compute */
        size_t ksize = (size_t)(k);    /* cast for indexing */

        /* ensure arrays large enough for new digit and new convolutions */
        ensure_digits(ksize + 1);
        ensure_prod(2 * ksize + 2);

        /* coefficient of 11^k in X² after normalisation */
        uint32_t coeff_k = prod[ksize] % 11;
        uint8_t digit_k = (uint8_t)((11 - coeff_k) % 11);
        digits[ksize] = digit_k;

        /* update convolution array with new digit contributions */
        for (size_t j = 0; j < ksize; ++j) {
            prod[ksize + j] += 2u * (uint32_t)digit_k * (uint32_t)digits[j];
        }
        prod[2 * ksize] += (uint32_t)digit_k * (uint32_t)digit_k;

        /* propagate carries from position k upwards */
        size_t pos = ksize;
        size_t max_index = 2 * ksize;
        while (1) {
            uint32_t carry = prod[pos] / 11;
            prod[pos] %= 11;
            if (carry == 0 && pos > max_index) break;
            ++pos;
            ensure_prod(pos + 1);      /* guarantee prod[pos] exists */
            prod[pos] += carry;
        }

        ++computed;
    }

    return (seq_t)digits[(size_t)n];
}
