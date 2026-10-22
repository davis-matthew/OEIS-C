/*
 * A346002  Distance 2 lexicode over the alphabet {0,1,2}, with the codewords
 *          written in base 10.
 * https://oeis.org/A346002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value of the n‑th distance‑2 lexicode word over the *
 *            alphabet {0,1,2}, interpreted as a ternary number and converted
 *            to * base‑10. The sequence is built greedily: the
 *            lexicographically * earliest word that maintains Hamming
 *            distance ≥2 from all previous * words is appended. See OEIS
 *            A346002
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Internal structures and state ---------- */

typedef struct {
    seq_t  val;              /* value of the word interpreted in base 3 */
    unsigned len;            /* number of base‑3 digits (MSB first)     */
    uint8_t digits[32];      /* the digits 0,1,2 of the word            */
} CodeWord;

static CodeWord *cache   = NULL;   /* memoised codewords                  */
static size_t   cache_sz = 0;      /* number of stored words              */
static size_t   cache_cap= 0;      /* current allocation capacity       */

/* Generator state for lexicographic enumeration of candidate words
 * (except the first word “0”). */
static unsigned    gen_len         = 1;  /* current candidate length       */
static int         gen_first_digit = 1;  /* most‑significant digit 1 or 2  */
static idx_t       gen_tail        = 0;  /* remaining digits (base‑3 encoded)  */

/* Powers of 3 used to decode gen_tail into decimal digits.  */
static idx_t pow3[64] = { 1 };
static size_t pow3_len = 1;

static void init_pow3(void)
{
    if (pow3_len > 1) return;
    for (size_t i = 1; i < sizeof(pow3)/sizeof(pow3[0]); ++i) {
        if (pow3[i-1] > ((idx_t)-1)/3) break;
        pow3[i] = pow3[i-1] * 3;
    }
    pow3_len = sizeof(pow3)/sizeof(pow3[0]);
}

/* ---------- Helper functions ---------- */

/* Check that a new candidate word has Hamming distance ≥2 from all
 * previously stored words.  Missing trailing positions of the
 * shorter word are taken as digit 0 (i.e. alignment from the right
 * (least‑significant) end). */
static bool candidate_ok(const CodeWord *cand)
{
    for (size_t k = 0; k < cache_sz; ++k) {
        const CodeWord *old = &cache[k];
        size_t maxlen = cand->len > old->len ? cand->len : old->len;
        int diff = 0;
        for (size_t p = 0; p < maxlen; ++p) {
            if (diff >= 2) break;
            uint8_t a = (cand->len > p)
                        ? cand->digits[cand->len-1-p]
                        : 0;
            uint8_t b = (old->len  > p)
                        ? old->digits[old->len-1-p]
                        : 0;
            if (a != b) ++diff;
        }
        if (diff < 2) return false;           /* too close to this word */
    }
    return true;
}

/* Ensure that the cache has at least 'n' terms.  Generates new terms
 * until the required index is reached. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) { /* first initialisation */
        cache_cap = 16;
        cache = (CodeWord *)malloc(cache_cap * sizeof(CodeWord));
        cache_sz = 0;

        /* first term a(1) = 0 (base‑3 word “0”) */
        CodeWord w;
        w.val   = 0;
        w.len   = 1;
        w.digits[0] = 0;
        cache[0] = w;
        cache_sz = 1;

        /* generator state already set to produce '1' as next candidate */
        gen_len        = 1;
        gen_first_digit= 1;
        gen_tail       = 0;

        init_pow3();
    }

    while (cache_sz < n) {
        /* Build candidate word */
        CodeWord cand;
        cand.len = (unsigned)gen_len;
        cand.digits[0] = (uint8_t)gen_first_digit;

        if (gen_len > 1) {
            idx_t tail = gen_tail;
            size_t d = cand.len - 1;        /* number of tail digits */
            for (size_t pos = 1; pos <= d; ++pos) {
                cand.digits[pos] =
                    (uint8_t)((tail / pow3[d - pos]) % 3);
            }
        }

        /* compute value of the word interpreted in base‑3 */
        seq_t val = 0;
        for (size_t i = 0; i < cand.len; ++i) {
            val = val * 3 + (seq_t)cand.digits[i];
        }
        cand.val = val;

        /* test candidate using distance condition */
        if (candidate_ok(&cand)) {
            /* store into cache, grow if necessary */
            if (cache_sz >= cache_cap) {
                cache_cap *= 2;
                cache = (CodeWord *)realloc(cache,
                                            cache_cap * sizeof(CodeWord));
            }
            cache[cache_sz] = cand;
            ++cache_sz;
        }

        /* advance to next candidate in lexicographic order */
        if (gen_len == 1) {   /* length 1: words '1' and '2' */
            if (gen_first_digit < 2)
                ++gen_first_digit;
            else {
                gen_len = 2;
                gen_first_digit = 1;
                gen_tail = 0;
            }
        } else {              /* length ≥2 */
            ++gen_tail;
            idx_t limit = pow3[gen_len - 1] - 1;
            if (gen_tail > limit) {
                gen_tail = 0;
                if (gen_first_digit < 2)
                    ++gen_first_digit;
                else {
                    ++gen_len;
                    gen_first_digit = 1;
                    gen_tail       = 0;
                }
            }
        }
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; sequence is 1‑based */
    ensure_cache(n);
    return cache[n - 1].val;
}
