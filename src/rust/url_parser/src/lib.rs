#![forbid(unsafe_code)]

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ParsedUrl {
  pub scheme: String,
  pub host: String,
  pub path: String,
}

pub fn parse_url(input: &str) -> Result<ParsedUrl, String> {
  let s = input.trim();
  if s.is_empty() {
    return Err("empty url".into());
  }

  let (scheme, rest) = match s.split_once("://") {
    Some((a, b)) => (a.to_lowercase(), b),
    None => return Err("missing scheme (expected ://)".into()),
  };

  if scheme != "http" && scheme != "https" && scheme != "about" {
    return Err(format!("unsupported scheme: {}", scheme));
  }

  if scheme == "about" {
    return Ok(ParsedUrl { scheme, host: "".into(), path: rest.to_string() });
  }

  let (host, path) = match rest.split_once('/') {
    Some((h, p)) => (h, format!("/{}", p)),
    None => (rest, "/".into()),
  };

  if host.is_empty() {
    return Err("empty host".into());
  }

  Ok(ParsedUrl { scheme, host: host.to_string(), path })
}

#[cfg(test)]
mod tests {
  use super::*;
  #[test]
  fn ok_http() {
    let u = parse_url("https://example.com/abc").unwrap();
    assert_eq!(u.scheme, "https");
    assert_eq!(u.host, "example.com");
    assert_eq!(u.path, "/abc");
  }
}