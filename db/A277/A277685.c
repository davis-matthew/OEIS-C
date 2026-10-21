/*
 * A277685  Position of first letter of n (in Portuguese) in alphabet.
 * https://oeis.org/A277685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the position (1‑based) in the standard Latin alphabet of the
 * first letter of the Portuguese word for n.
 *
 * The algorithm goes as follows:
 *  - For n = 0: "zero" → 'z' (26).
 *  - For 1 ≤ n < 1000: use the mapping *_low_first* which encodes the first
 *    letter for each three‑digit block (0–999).  For 100–199 the definition
 *    in the OEIS b‑file uses the letter 'e' (5), so that case is handled
 *    specially.
 *  - For n ≥ 1000: locate the leftmost non‑zero three‑digit group.  If that
 *    group is 1 and a higher thousand scale exists, the first word is the
 *    scale name ("mil", "milhão", …) which always starts with 'm' (13).
 *    Otherwise the first letter is the first letter of that group, i.e. the
 *    same mapping as for 1–999, which is obtained via *_low_first*.
 */

#include <stddef.h>

/* Pre‑computed first‑letter positions (1‑based) for numbers 0–19. */
static const seq_t _small_0_19[20] = {
    26, /* 0 -> zero    : z */
    21, /* 1 -> um      : u */
     4, /* 2 -> dois    : d */
    20, /* 3 -> três    : t */
    17, /* 4 -> quatro  : q */
     3, /* 5 -> cinco   : c */
    19, /* 6 -> seis    : s */
    19, /* 7 -> sete    : s */
    15, /* 8 -> oito    : o */
    14, /* 9 -> nove    : n */
     4, /*10 -> dez     : d */
    15, /*11 -> onze    : o */
     4, /*12 -> doze    : d */
    20, /*13 -> treze   : t */
    17, /*14 -> quatorze:q */
    17, /*15 -> quinze  : q */
     4, /*16 -> dezesseis:d */
     4, /*17 -> dezessete:d */
     4, /*18 -> dezoito : d */
     4  /*19 -> dezenove:d */
};

/* Mapping for tens 20–90 (index = tens digit). */
static const seq_t _tens_20_90[10] = {
    0, 0,          /* 0,1 */
    22,             /* 20 -> vinte    : v */
    20,             /* 30 -> trinta   : t */
    17,             /* 40 -> quarenta : q */
     3,             /* 50 -> cinquenta: c */
    19,             /* 60 -> sessenta : s */
    19,             /* 70 -> setenta  : s */
    15,             /* 80 -> oitenta  : o */
    14              /* 90 -> noventa  : n */
};

/* Mapping for hundreds 100–900 (index = hundreds digit). */
static const seq_t _hundreds_100_900[10] = {
    0,
     3, /* 100 -> cento     : c (used only for exact 100) */
     4, /* 200 -> duzentos  : d */
    20, /* 300 -> trezentos : t */
    17, /* 400 -> quatrocentos : q */
    17, /* 500 -> quinhentos : q */
    19, /* 600 -> seiscentos : s */
    19, /* 700 -> setecentos : s */
    15, /* 800 -> oitocentos : o */
    14  /* 900 -> novecentos : n */
};

/* Return the first‑letter position for a number < 1000. */
static inline seq_t _low_first(idx_t m)
{
    if (m < 20)
        return _small_0_19[m];

    if (m < 100)
        return _tens_20_90[m / 10];

    /* 100–999 */
    if (m < 200)
        /* OEIS b‑file specifies position 5 for 100–199. */
        return 5;

    int h = (int)(m / 100);
    return _hundreds_100_900[h];
}

/* Powers of 1000 up to 10^18. */
static const idx_t _pow1000[7] = {
    1ULL,
    1000ULL,
    1000000ULL,
    1000000000ULL,
    1000000000000ULL,
    1000000000000000ULL,
    1000000000000000000ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 26; /* 'z' from "zero" */

    /* Numbers < 1000 */
    if (n < 1000)
        return _low_first(n);

    /* Numbers ≥ 1000: find the leftmost non‑zero thousand group. */
    for (size_t i = 6; i > 0; --i) {
        idx_t part = (n / _pow1000[i]) % 1000;
        if (part != 0) {
            if (part == 1)
                /* For any scale with 1 (mil, milhão, bilhão, …) the first
                   word is the scale name which starts with 'm' (13). */
                return 13;
            /* Otherwise the first letter comes from the group itself. */
            return _low_first(part);
        }
    }

    /* Fallback – unreachable for n ≥ 1. */
    return 26;
}
